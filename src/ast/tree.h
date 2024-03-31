#ifndef DONSUS_TREE_H
#define DONSUS_TREE_H
#include <assert.h>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <vector>

#include "../utility/handle.h"
#include "../utility/memory_alloc.h"
#include "node.h"
#include "parser_util.h"
#include "symbol_table.h"

// Holds top level ast nodes
/* (old version)
 * std::vector<std::variant<donsus_math_expr*, donsus_variable_decl*>> body;
 *
 * *\
 */

namespace DonsusCodegen {
class DonsusCodeGenerator; // forward reference
}
namespace donsus_ast {

class tree {
public:
  tree();
  // TBD void traverse()
  void add_node(utility::handle<node>);
  auto get_nodes() -> std::vector<utility::handle<node>>;
  auto get_allocator() -> utility::DonsusAllocator;
  void allocate_node_list(uint64_t count);

  // -------------------------------------------------------
  // https://en.wikipedia.org/wiki/Tree_traversal
  void traverse(std::function<void(utility::handle<node>,
                                   utility::handle<DonsusSymTable> table)>
                    visit,
                std::function<void(utility::handle<node>)> assign_node,
                utility::handle<DonsusSymTable> sym,
                DonsusCodegen::DonsusCodeGenerator &codegen,
                utility::handle<node> curr_node = nullptr);
  // FOR DEBUGGING
  void traverse(std::function<void(utility::handle<node>,
                                   utility::handle<DonsusSymTable> table)>
                    visit,
                std::function<void(utility::handle<node>)> assign_node,
                utility::handle<DonsusSymTable> sym,
                utility::handle<node> curr_node = nullptr);

  // implement traverse and use stack
  void traverse_nodes(std::function<void(utility::handle<node>,
                                         utility::handle<DonsusSymTable> table)>
                          visit,
                      std::function<void(utility::handle<node>)> assign_node,
                      utility::handle<DonsusSymTable> sym,
                      DonsusCodegen::DonsusCodeGenerator &codegen,
                      utility::handle<node> curr_node = nullptr);
  void evaluate(std::function<void(utility::handle<node>,
                                   utility::handle<DonsusSymTable> table)>
                    visit,
                std::function<void(utility::handle<node>)> assign_node,
                utility::handle<DonsusSymTable> sym,
                DonsusCodegen::DonsusCodeGenerator &codegen,
                utility::handle<node> curr_node = nullptr);

  void init_traverse();
  // -------------------------------------------------------
  template <typename extra_type>
  auto create_node(donsus_node_type type, uint64_t child_count)
      -> utility::handle<node> {
    assert(child_count <= std::numeric_limits<uint64_t>::max());
    const utility::handle<node> node_ptr = allocator.r_alloc<node>();

    // initialise the node
    allocate_node_list(child_count); // reserve space for children
    // type
    node_ptr->type = type;
    node_ptr->children =
        {}; // initialise it as an empty vector rather than the nodes
    node_ptr->set_property(allocator.r_alloc<extra_type>());
    return node_ptr;
  };

private:
  std::vector<utility::handle<node>> nodes;
  utility::DonsusAllocator allocator;
  std::stack<utility::handle<node>> stack_assign; // traverse
  std::queue<utility::handle<node>> stack_visit;  // traverse
};
} // namespace donsus_ast

#endif // DONSUS_TREE_H