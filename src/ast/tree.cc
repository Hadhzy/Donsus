#include "tree.h"

using namespace donsus_ast;

tree::tree() : allocator(1024) {}

void tree::add_node(utility::handle<node> node) { nodes.push_back(node); }


// AST WALKER
void tree::traverse(std::function<void(utility::handle<node>, utility::handle<DonsusSymTable> table)>visit,
                    utility::handle<DonsusSymTable> sym) const{
  std::stack<utility::handle<node>> stack;

  for (auto& n: nodes){
     stack.push(n);
     while (!stack.empty()){

       auto current = stack.top();
       stack.pop();

       if (!current) {
         continue;
       }
         // call visit with node symbol table
        visit(current, sym);
         // process child
        for (auto c: current->children){
          stack.push(c);
       }
     }
  }
}

auto tree::get_nodes() -> std::vector<utility::handle<node>> { return nodes; }

auto tree::get_allocator() -> utility::DonsusAllocator { return allocator; }

void tree::allocate_node_list(uint64_t count) {
  nodes.reserve(count); // reserve memory for a specific count
}