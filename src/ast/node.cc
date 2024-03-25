#include "node.h"

using namespace donsus_ast;

donsus_node_type::donsus_node_type(underlying type) : type(type) {}

auto donsus_node_type::to_string() const -> std::string {
  switch (type) {
  case DONSUS_VARIABLE_DECLARATION:
    return "DONSUS_VARIABLE_DECLARATION";
  case DONSUS_NUMBER_EXPRESSION:
    return "DONSUS_NUMBER_EXPRESSION";
  case DONSUS_VARIABLE_DEFINITION:
    return "DONSUS_VARIABLE_DEFINITION";
  case DONSUS_FUNCTION_DECL:
    return "DONSUS_FUNCTION_DECLARATION";
  case DONSUS_FUNCTION_DEF:
    return "DONSUS_FUNCTION_DEFINITION";
  case DONSUS_ASSIGNMENT:
    return "DONSUS_ASSIGMENT";
  case DONSUS_IDENTIFIER:
    return "DONSUS_IDENTIFIER";
  case DONSUS_IF_STATEMENT:
    return "DONSUS_IF_STATEMENT";
  case DONSUS_EXPRESSION:
    return "DONSUS_EXPRESSION";
  case DONSUS_ELSE_STATEMENT:
    return "DONSUS_ELSE_STATEMENT";
  case DONSUS_FUNCTION_CALL:
    return "DONSUS_FUNCTION_CALL";
  case DONSUS_RETURN_STATEMENT:
    return "DONSUS_RETURN_STATEMENT";
  case DONSUS_STRING_EXPRESSION:
    return "DONSUS_STRING_EXPRESSION";
  }
}

std::string
donsus_ast::de_get_from_donsus_node_type(donsus_ast::donsus_node_type type) {
  switch (type.type) {
  case donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    return "DONSUS_VARIABLE_DECLARATION";
  }
  case donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
    return "DONSUS_NUMBER_EXPRESSION";
  }
  case donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
    return "DONSUS_VARIABLE_DEFINITION";
  }
  case donsus_node_type::DONSUS_FUNCTION_DECL: {
    return "DONSUS_FUNCTION_DECLARATION";
  }
  case donsus_node_type::DONSUS_FUNCTION_DEF: {
    return "DONSUS_FUNCTION_DEFINITION";
  }
  case donsus_node_type::DONSUS_IF_STATEMENT: {
    return "DONSUS_IF_STATEMENT";
  }
  case donsus_node_type::DONSUS_IDENTIFIER: {
    return "DONSUS_IDENTIFIER";
  }
  case donsus_node_type::DONSUS_ASSIGNMENT: {
    return "DONSUS_ASSIGNMENT";
  }
  case donsus_node_type::DONSUS_EXPRESSION: {
    return "DONSUS_EXPRESSION";
  }

  case donsus_node_type::DONSUS_ELSE_STATEMENT: {
    return "DONSUS_ELSE_STATEMENT";
  }

  case donsus_node_type::DONSUS_FUNCTION_CALL: {
    return "DONSUS_FUNCTION_CALL";
  }

  case donsus_node_type::DONSUS_RETURN_STATEMENT: {
    return "DONSUS_RETURN_STATEMENT";
  }
  case donsus_node_type::DONSUS_STRING_EXPRESSION: {
    return "DONSUS_STRING_EXPRESSION";
  }

  default: {
  }
  }
}