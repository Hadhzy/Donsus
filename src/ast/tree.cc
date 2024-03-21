#include "tree.h"

using namespace donsus_ast;

// sym_table helpers
void add_params_sym(utility::handle<DonsusSymTable> sym,
                    std::vector<NAME_DATA_PAIR> children) {
  for (auto &n : children) {
    // add params to a specific symbol table
    sym->add(n.identifier);
  }
}

tree::tree() : allocator(1024) {}

void tree::add_node(utility::handle<node> node) { nodes.push_back(node); }

// Recursion is not possible
// AST WALKER
void tree::traverse(
    const std::function<void(utility::handle<node>,
                             utility::handle<DonsusSymTable> table)> &visit,
    const std::function<void(utility::handle<node>)> &assign_node,
    utility::handle<DonsusSymTable> sym) const {
  std::stack<utility::handle<node>> stack_first;
  std::stack<utility::handle<node>> stack_second;

  for (auto &n : nodes) {
    stack_first.push(n);
    stack_second.push(n);

    // global nodes
    switch (n->type.type) {
    case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
      auto stuff = n->get<donsus_ast::function_def>();
      auto b = sym->add_sym_table(stuff.func_name);

      // process children
      add_params_sym(b, stuff.parameters);
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
      auto stuff = n->get<donsus_ast::variable_decl>();
      sym->add(stuff.identifier_name);
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
      auto stuff = n->get<donsus_ast::variable_decl>();
      sym->add(stuff.identifier_name);
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
      auto stuff = n->get<donsus_ast::function_decl>();
      sym->add(stuff.func_name);
      break;
    }

      /*    case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
            break;
          }*/

      /*    case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT: {

            break;
          }*//*
    case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER: {
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT: {
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
    }*/
    default: {
    }
    }

    while (!stack_first.empty()) {

      auto current = stack_first.top();
      stack_first.pop();

      if (!current) {
        continue;
      }

      // call visit with node symbol table
      assign_node(current); // call function donsus_sym in sema.cc
                            // process child
      // if(
      for (auto c : current->children) {
        stack_first.push(c);
      }
    }

    while (!stack_second.empty()) {

      auto current = stack_second.top();
      stack_second.pop();

      if (!current) {
        continue;
      }

      // call visit with node symbol table
      visit(current, sym); // call function donsus_sym in sema.cc
                           // process child
      // if(
      for (auto c : current->children) {
        stack_second.push(c);
      }
    }
  }
}

auto tree::get_nodes() -> std::vector<utility::handle<node>> { return nodes; }

auto tree::get_allocator() -> utility::DonsusAllocator { return allocator; }

void tree::allocate_node_list(uint64_t count) {
  // nodes.reserve(count); // reserve memory for a specific count
}