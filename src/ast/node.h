#ifndef donsus_node_h
#define donsus_node_h
#include <iostream>
#include <variant>
#include <vector>

#include "../../Include/token.h"
#include "../Include/Internal/type.h"
#include "../utility/handle.h"
#include "../utility/property.h"
#include "../utility/slices.h"
// Contains ast(node) types that the parser figures out

namespace donsus_ast {
struct donsus_node_type {
  enum underlying : int {
    DONSUS_VARIABLE_DECLARATION, // just the type of the node
    DONSUS_VARIABLE_DEFINITION,  // just the type of the node
    DONSUS_FUNCTION_DECL,
    DONSUS_NUMBER_EXPRESSION // just the type of the node
  };

  donsus_node_type() = default;
  donsus_node_type(underlying type);

  [[nodiscard]] auto to_string() const -> std::string;
  underlying type;
};

std::string de_get_from_donsus_node_type(donsus_node_type type);

// actual node structure containing extra properties
struct variable_decl {
  donsus_token_kind identifier_type;
  std::string identifier_name;
  void *identifier_value;
};
// actual node structure containing extra properties
struct number_expr {
  donsus_token value;
};

// actual node structure containing extra properties
struct function_decl {
  DONSUS_TYPE return_type; // the return type of the function

  // function signature
  std::vector<NAME_DATA_PAIR>
      parameters; // parameters inside the function signature

  std::string func_name; // name of the function
};

using node_properties =
    utility::property<variable_decl, number_expr, function_decl>;

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