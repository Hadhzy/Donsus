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
    DONSUS_FUNCTION_DECL,        // just the type of the node
    DONSUS_FUNCTION_DEF,         // just the type of the node
    DONSUS_IF_STATEMENT,         // just the type of the node
    DONSUS_ASSIGNMENT,           // just the type of the node
    DONSUS_IDENTIFIER,           // just the type of the node
    DONSUS_NUMBER_EXPRESSION,    // just the type of the node
    DONSUS_EXPRESSION,           // just the type of the node
    DONSUS_FUNCTION_CALL,        // just the type of the node
    DONSUS_ELSE_STATEMENT,       // just the type of the node
    DONSUS_RETURN_STATEMENT,     // just the type of the node
  };

  donsus_node_type() = default;
  donsus_node_type(underlying type);

  [[nodiscard]] auto to_string() const -> std::string;
  underlying type;
};

std::string de_get_from_donsus_node_type(donsus_node_type type);

// Forward declaration of the node structure
struct node;

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
  //
  std::vector<DONSUS_TYPE> return_type; // the return type of the function

  // function signature
  std::vector<NAME_DATA_PAIR>
      parameters; // parameters inside the function signature

  std::string func_name; // name of the function
};

struct function_def {
  std::vector<DONSUS_TYPE> return_type; // the return type of the function

  // function signature
  std::vector<NAME_DATA_PAIR>
      parameters; // parameters inside the function signature

  std::string func_name; // name of the function

  std::vector<utility::handle<donsus_ast::node>>
      body; // This might be refactored in the future
};

struct function_call {
  // test(1,2,3,4,5,6)
  // test ->func_name
  // (1,2,3,4,5,6) ->arguments
  std::string func_name;
  std::vector<NAME_OR_DATA_PAIR> arguments;
};

struct if_statement {
  std::vector<utility::handle<donsus_ast::node>> body;
  // we will include something for the init_statement_condition
  // the children represents the condition
  // the body represents the block when the condition evaluates to true
  std::vector<utility::handle<donsus_ast::node>>
      alternate; // this represents the block to be executed if the condition
                 // evaluates as false(else or elif statement)
};

struct else_statement {
  std::vector<utility::handle<donsus_ast::node>> body;
};

struct return_kw {
  std::vector<DONSUS_TYPE> types; // only if the main type is not long enough
};

/*
assignment: | assignment_start assignment_op assignment_value+
 * */
struct assignment {
  std::string identifier_name;
  donsus_token op; // operator
};

struct identifier {
  std::string identifier_name; //  name of lvalue
};

struct expression {
  donsus_token value;
};

using node_properties =
    utility::property<variable_decl, number_expr, function_decl, function_def>;

struct node : node_properties {
  // children tbd
  std::vector<utility::handle<donsus_ast::node>>
      children;          // size type in the future
  donsus_node_type type; // This is the node's type
  DONSUS_TYPE real_type; // This type is assigned during type checking
};
} // namespace donsus_ast
#endif