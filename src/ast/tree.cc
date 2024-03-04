#include "tree.h"

using namespace donsus_ast;

// sym_table helpers
void add_params_sym(utility::handle<DonsusSymTable> sym,
                    std::vector<NAME_DATA_PAIR> children) {
  for (auto n : children) {
    // add params to a specific symbol table
    sym->add(n.identifier);
  }
}

tree::tree() : allocator(1024) {}

void tree::add_node(utility::handle<node> node) { nodes.push_back(node); }

// AST WALKER
void tree::traverse(std::function<void(utility::handle<node>,
                                       utility::handle<DonsusSymTable> table)>
                        visit,
                    utility::handle<DonsusSymTable> sym) const {
  std::stack<utility::handle<node>> stack;
  for (auto &n : nodes) {
    stack.push(n);
    // global nodes

    switch (n->type.type) {
    // Todo: shouldnt use auto, GET RID OF get<>
    case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
      auto _stuff = n->get<donsus_ast::function_def>();
      auto b = sym->add_sym_table(_stuff.func_name);

      // process children
      add_params_sym(b, _stuff.parameters);
      break;
    }
    default: {
    }
    }

    while (!stack.empty()) {

      auto current = stack.top();
      stack.pop();

      if (!current) {
        continue;
      }

      // call visit with node symbol table
      visit(current, sym); // call function donsus_sym in sema.cc
                           // process child
      for (auto c : current->children) {
        stack.push(c);
      }
    }
  }
}

auto tree::get_nodes() -> std::vector<utility::handle<node>> { return nodes; }

auto tree::get_allocator() -> utility::DonsusAllocator { return allocator; }

void tree::allocate_node_list(uint64_t count) {
  // nodes.reserve(count); // reserve memory for a specific count
}