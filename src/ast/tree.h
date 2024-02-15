#ifndef DONSUS_TREE_H
#define DONSUS_TREE_H
#include <assert.h>
#include <iostream>
#include <limits>
#include <vector>

#include "../utility/handle.h"
#include "../utility/memory_alloc.h"
#include "node.h"

// Holds top level ast nodes
/* (old version)
 * std::vector<std::variant<donsus_math_expr*, donsus_variable_decl*>> body;
 *
 * *\
 */

namespace donsus_ast {
class tree {
public:
  tree();
  // TBD void traverse()
  void add_node(utility::handle<node>);
  auto get_nodes() -> std::vector<utility::handle<node>>;
  auto get_allocator() -> utility::DonsusAllocator;

  auto allocate_node_list(uint64_t count) -> std::vector<utility::handle<node>>;

  auto create_node(donsus_node_type type, uint64_t child_count, donsus_token value)
      -> utility::handle<node> {
    assert(child_count <= std::numeric_limits<uint64_t>::max());
    // Todo: think about this
    const utility::handle node_ptr = allocator.emplace<node>();

    // initialise the node
    node_ptr->type = type;
    node_ptr->children = allocate_node_list(child_count);
    node_ptr->value = value;

    return node_ptr;
  };

private:
  std::vector<utility::handle<node>> nodes;
  utility::DonsusAllocator allocator;
};
} // namespace donsus_ast

#endif // DONSUS_TREE_H