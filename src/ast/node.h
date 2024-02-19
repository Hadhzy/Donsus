#ifndef donsus_node_h
#define donsus_node_h
#include "../../Include/token.h"
#include "../utility/handle.h"
#include "../utility/property.h"
#include "../utility/slices.h"
#include <iostream>
#include <variant>
#include <vector>

// Contains ast(node) types that the parser figures out

namespace donsus_ast {
struct donsus_node_type {
  enum underlying : int {
    DONSUS_VARIABLE_DECLARATION,
    DONSUS_VARIABLE_DEFINITION,
    DONSUS_NUMBER_EXPRESSION
  };

  donsus_node_type() = default;
  donsus_node_type(underlying type);

  [[nodiscard]] auto to_string() const -> std::string;
  underlying type;
};

std::string de_get_from_donsus_node_type(donsus_node_type type);

struct variable_decl {
  donsus_token_kind identifier_type;
  std::string identifier_name;
  void *identifier_value;
};

struct number_expr {
  donsus_token value;
};

using node_properties = utility::property<variable_decl, number_expr>;

struct node : node_properties {
  // children tbd
  std::vector<utility::handle<donsus_ast::node>>
      children; // size type in the future
  donsus_node_type type;

  // For debugging purposes
  friend std::ostream &operator<<(std::ostream &o, node &node) {
    o << "type: " << de_get_from_donsus_node_type(node.type); // type
    o << "\n";

    // children
    for (auto n : node.children) {
      o << "children: ";
      o << *n; // recursion
    }
    return o;
  }
};
} // namespace donsus_ast
#endif