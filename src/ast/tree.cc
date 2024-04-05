#include "tree.h"
#include "../../Include/codegen/codegen.h"
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
void tree::traverse(
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table,
                       utility::handle<DonsusSymTable> global_table)>
        visit,
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table,
                       utility::handle<DonsusSymTable> global_table)>
        assign_node,
    utility::handle<DonsusSymTable> sym, utility::handle<node> curr_node) {

  DonsusCodegen::DonsusCodeGenerator codegen;
  traverse(visit, assign_node, sym, codegen, curr_node);
}

void tree::traverse(
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table,
                       utility::handle<DonsusSymTable> global_table)>
        visit,
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table,
                       utility::handle<DonsusSymTable> global_table)>
        assign_node,
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
    std::function<void(utility::handle<node>, utility::handle<DonsusSymTable>,
                       utility::handle<DonsusSymTable>)>
        visit,
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table,
                       utility::handle<DonsusSymTable> global_table)>
        assign_type_to_node,
    utility::handle<DonsusSymTable> sym,
    DonsusCodegen::DonsusCodeGenerator &codegen, utility::handle<node> n) {

  // global nodes
  switch (n->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
    auto stuff = n->get<donsus_ast::function_def>();
    auto b = sym->add_sym_table(stuff.func_name);
    b->function_return_type = stuff.return_type;

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

/*
Calls specific visit functions on the AST nodes.
Depth first search.
Loop through the stack that is already filled up with ITEMS.
stack_assign: assign types to nodes - data member
stack_visit: performs typechecking - data member
 * */
void tree::evaluate(
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table,
                       utility::handle<DonsusSymTable> global_table)>
        visit,
    std::function<void(utility::handle<node>,
                       utility::handle<DonsusSymTable> table,
                       utility::handle<DonsusSymTable> global_table)>
        assign_type_to_node,
    utility::handle<DonsusSymTable> sym,
    DonsusCodegen::DonsusCodeGenerator &codegen,
    utility::handle<node> curr_node) {
  // Assign nodes to types
  while (!stack_assign.empty()) {
    // python pop
    auto current = stack_assign.top();
    stack_assign.pop();

    if (!current) {
      continue;
    }

    assign_type_to_node(current, sym, sym);

    if (current->type.type == donsus_node_type::DONSUS_FUNCTION_DEF) {
      for (auto c : current->get<function_def>().body) {
        if (c->type.type == donsus_node_type::DONSUS_ASSIGNMENT) {
          continue;
        } else {
          stack_assign.push(c);
        }
      }
    }
    for (auto c : current->children) {
      stack_assign.push(c);
    }
  }

  // Doing typechecking
  while (!stack_visit.empty()) {
    // python pop
    auto current = stack_visit.front();
    stack_visit.pop();

    if (!current) {
      continue;
    }

    if (current->type.type == donsus_node_type::DONSUS_FUNCTION_DEF) {
      std::string func_name =
          current->get<donsus_ast::function_def>().func_name;
      std::string qualified_name = sym->apply_scope(func_name);
      auto sym_table = sym->get_sym_table(qualified_name);

      visit(current, sym_table, sym);
      // when called by test api, it'll be false
      if (codegen.Builder) {
        codegen.compile(current, sym_table);
      }

    } else {
      visit(current, sym, sym);
      // when called by test api, it'll be false
      if (codegen.Builder) {
        codegen.compile(current, sym);
      }
    }
  }
  // when called by test api, it'll be false
  if (codegen.Builder) {
    // CREATE RET instruction
    codegen.Finish();
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