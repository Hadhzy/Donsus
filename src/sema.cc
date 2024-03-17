// Construct symbol table from ast(1)
// Semantic analysis generates SEA OF NODES from symbol table(2) or AST(TBD)
/* \*
 *    AST(no):
 *      +
 *     /  \
 *    2    3
 *
 *
 *      name     type    scope
 *     |-----------------------|
 *     |       |        |      |
 *     |-----------------------|
 *     |       |        |      |
 *     |-----------------------|
 *     |       |        |      |
 *     |-----------------------|
 *
 */
/*
 Type checking TODOS:
  - check whether var decl and def has the same type
  if the value is not a constant literal we could just check for the function.
  - check if the number of return values matches the one in the function
 prototype
  - check if the if statement's init statement condition evaluates to bool
  - don't allow redefinition

  unsigned long int a = 12 + "sdfsd" + func_call();
  - figure out if the if statement is true
 */

#include "../Include/sema.h"

DonsusSema sema;

// utility

//  Typechecking
void donsus_sym(utility::handle<donsus_ast::node> node,
                utility::handle<DonsusSymTable> table) {
  /*
   Call type checking function with the correct type
   **/
  switch (node->type.type) {

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    sema.donsus_sema_is_defined(
        make_type(node->get<donsus_ast::variable_decl>().identifier_type)
            .type_un,
        table);
  }
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
    // make sure it is not defined already
    // make sure the type is correct
  }

  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
    // (a<4+5)
    node->real_type = sema.donsus_typecheck_type_from_node(node->type);

    sema.donsus_typecheck_type_is_bool_conversion(node->children[0]);
    // see if the operations are supported
    sema.donsus_typecheck_support_between_types(node->children[0]);
  }

  default: {
  }
  }
}

void DonsusSema::donsus_sema(utility::handle<donsus_ast::node> ast) {
  // Entry Point
}

auto DonsusSema::donsus_sema_is_defined(donsus_type ast,
                                        utility::handle<DonsusSymTable> table)
    -> void {
  // check if the
}

/**
 * \brief Checks if the 2 types are compatible.
 */
auto DonsusSema::donsus_typecheck_is_compatible(
    utility::handle<donsus_ast::node> first,
    utility::handle<donsus_ast::node> second) -> bool {

  // call == overload
  if (first->real_type == second->real_type) {
    return true;
  }
  throw DonsusException("Operation between: " + first->real_type.to_string() +
                        " and:" + second->real_type.to_string() +
                        "are not supported");
}

/**
 * \brief Checks if the type of the node is contextually bool convertable.
 */
auto DonsusSema::donsus_typecheck_type_is_bool_conversion(
    utility::handle<donsus_ast::node> node) -> bool {
  switch (node->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL:
  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER:

    return true;

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION:
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF:
  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT:
  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT:
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT:
    return false;
  }
}

/**
 * \brief Returns the type of the expr.
 */
auto DonsusSema::donsus_typecheck_type_expr(
    utility::handle<donsus_ast::node> node) -> DONSUS_TYPE {}

/**
 * \brief Check if the operators are supported between operands.
 */
auto DonsusSema::donsus_typecheck_support_between_types(
    // get the highest node
    // see if its children are compatible move to next children
    utility::handle<donsus_ast::node> node, int level)
    -> utility::handle<donsus_ast::node> {

  utility::handle<donsus_ast::node> middle;
  if (!node->children[0]) {
    return node;
  }

  if (level == 0) {
    level++;
    middle = node->children[0];
  } else {
    middle = node;
  }

  donsus_typecheck_is_compatible(middle, middle->children[0]);
  donsus_typecheck_is_compatible(middle, middle->children[1]);
}

auto DonsusSema::donsus_typecheck_type_from_node(
    donsus_ast::donsus_node_type type) -> DONSUS_TYPE {
  DONSUS_TYPE type_to_node{};

  switch (type.type) {
  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
    type_to_node.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
  }

  default: {
  }
  }
  return type_to_node;
}
