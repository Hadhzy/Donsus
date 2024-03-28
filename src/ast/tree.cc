#include "tree.h"
#include "../../Include/codegen/codegen.h"

#define IS_DEBUG 0
using namespace donsus_ast;

// sym_table helpers
void add_params_sym(utility::handle<DonsusSymTable> sym,
                    std::vector<NAME_DATA_PAIR> &children) {
  for (auto &n : children) {
    // add params to a specific symbol table
    // n.type doesn't work here
    sym->add(n.identifier, n.type);
  }
}

tree::tree() : allocator(1024) {}

void tree::add_node(utility::handle<node> node) { nodes.push_back(node); }

// Recursion is not possible
// AST WALKER
void tree::traverse(std::function<void(utility::handle<node>,
                                       utility::handle<DonsusSymTable> table)>
                        visit,
                    std::function<void(utility::handle<node>)> assign_node,
                    utility::handle<DonsusSymTable> sym,
                    utility::handle<node> curr_node) {

  DonsusCodegen::DonsusCodeGenerator codegen;
#undef IS_DEBUG
#define IS_DEBUG 1
  traverse(visit, assign_node, sym, codegen, curr_node);
}

void tree::traverse(std::function<void(utility::handle<node>,
                                       utility::handle<DonsusSymTable> table)>
                        visit,
                    std::function<void(utility::handle<node>)> assign_node,
                    utility::handle<DonsusSymTable> sym,
                    DonsusCodegen::DonsusCodeGenerator &codegen,
                    utility::handle<node> curr_node) {
  if (curr_node) {
    if (curr_node->type.type ==
        donsus_node_type::underlying::DONSUS_FUNCTION_DEF) {
      for (auto &n : curr_node->get<donsus_ast::function_def>().body) {
        traverse_nodes(visit, assign_node, sym, codegen, n);
      }
    } else {
      for (auto &n : curr_node->children)
        traverse_nodes(visit, assign_node, sym, codegen, n);
    }
  } else {
    for (auto &n : nodes) {
      traverse_nodes(visit, assign_node, sym, codegen, n);
    }
  }

  if (curr_node) {
    if (curr_node->type.type ==
        donsus_node_type::underlying::DONSUS_FUNCTION_DEF) {
      for (auto &n : curr_node->get<donsus_ast::function_def>().body) {
        evaluate(visit, assign_node, sym, codegen, n);
      }
    } else {
      for (auto &n : curr_node->children)
        evaluate(visit, assign_node, sym, codegen, n);
    }
  } else {
    for (auto &n : nodes) {
      evaluate(visit, assign_node, sym, codegen, n);
    }
  }
}

void tree::traverse_nodes(
    std::function<void(utility::handle<node>, utility::handle<DonsusSymTable>)>
        visit,
    std::function<void(utility::handle<node>)> assign_type_to_node,
    utility::handle<DonsusSymTable> sym,
    DonsusCodegen::DonsusCodeGenerator &codegen, utility::handle<node> n) {

  // global nodes
  switch (n->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
    auto stuff = n->get<donsus_ast::function_def>();
    auto b = sym->add_sym_table(stuff.func_name);

    // process children
    add_params_sym(b, stuff.parameters);
    for (auto &children : n->get<donsus_ast::function_def>().body) {
      traverse_nodes(visit, assign_type_to_node, b, codegen, children);
    }
    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
    auto stuff = n->get<donsus_ast::variable_decl>();
    sym->add(stuff.identifier_name,
             make_type(n->get<donsus_ast::variable_decl>().identifier_type));
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    auto stuff = n->get<donsus_ast::variable_decl>();
    sym->add(stuff.identifier_name,
             make_type(n->get<donsus_ast::variable_decl>().identifier_type));
    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
    auto stuff = n->get<donsus_ast::function_decl>();
    sym->add(stuff.func_name, n->get<donsus_ast::function_decl>().return_type);
    break;
  }

  default: {
  }
  }
}

void tree::evaluate(
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table)>
        visit,
    std::function<void(utility::handle<node>)> assign_type_to_node,
    utility::handle<DonsusSymTable> sym,
    DonsusCodegen::DonsusCodeGenerator &codegen,
    utility::handle<node> curr_node) {
  while (!stack_assign.empty()) {
    auto current = stack_assign.top();
    stack_assign.pop();

    if (!current) {
      continue;
    }

    // if it is a function def go through its body
    assign_type_to_node(current);

    if (current->type.type == donsus_node_type::DONSUS_FUNCTION_DEF) {
      for (auto c : current->get<function_def>().body) {
        stack_assign.push(c);
      }
    }
    for (auto c : current->children) {
      stack_assign.push(c);
    }
  }

  while (!stack_visit.empty()) {
    auto current = stack_visit.front();
    stack_visit.pop();

    if (!current) {
      continue;
    }

    // if it is a function def go through its body
    if (current->type.type == donsus_node_type::DONSUS_FUNCTION_DEF) {
      std::string func_name =
          current->get<donsus_ast::function_def>().func_name;
      std::string qualified_name = sym->apply_scope(func_name);
      auto sym_table = sym->get_sym_table(qualified_name);
      // call it here
      visit(current, sym_table);
#ifndef DEBUG
      codegen.compile(current, sym_table);
#endif
    } else {
      visit(current, sym);
#ifndef DEBUG
      codegen.compile(current, sym);
#endif
    }

    // if (current->type.type == donsus_node_type::DONSUS_FUNCTION_DEF) {
    //   for (auto c : current->get<function_def>().body) {
    //     stack_visit.push(c);
    //   }
    // }
    for (auto c : current->children) {
      stack_visit.push(c);
    }
  }
}

void tree::init_traverse() {
  for (auto &n : nodes) {
    stack_assign.push(n);
    stack_visit.push(n);
  }
}

auto tree::get_nodes() -> std::vector<utility::handle<node>> { return nodes; }

auto tree::get_allocator() -> utility::DonsusAllocator { return allocator; }

void tree::allocate_node_list(uint64_t count) {
  // nodes.reserve(count); // reserve memory for a specific count
}