#ifndef donsus_node_h
#define donsus_node_h
#include <iostream>
#include <vector>

#include "../../Include/token.h"
#include "../utility/handle.h"
#include "../utility/slices.h"

// Contains ast(node) types that the parser figures out

namespace donsus_ast {
struct donsus_node_type {
  enum underlying : int {
    DONSUS_VARIABLE_DECLARATION,
    DONSUS_NUMBER_EXPRESSION
  };

  donsus_node_type() = default;
  donsus_node_type(underlying type);

  [[nodiscard]] auto to_string() const -> std::string;
  underlying type;
};
struct node {
  // children tbd
  std::vector<utility::handle<donsus_ast::node>>
      children; // size type in the future
  donsus_node_type type;
  donsus_token value;
};
} // namespace donsus_ast
#endif