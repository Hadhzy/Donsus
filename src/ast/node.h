#ifndef donsus_node_h
#define donsus_node_h
#include "../utility/handle.h"
// Contains ast(node) types that the parser figures out
#include <iostream>
#include <vector>

namespace donsus_ast {
struct donsus_node_type {
  enum underlying : int { DONSUS_VARIABLE_DECLARATION };
  donsus_node_type() = default;
  donsus_node_type(underlying type);

  auto to_string() const -> std::string;
  underlying type;
};
struct node {
  // children tbd
  std::vector<node> children;
  donsus_node_type type;
};
} // namespace donsus_ast
#endif