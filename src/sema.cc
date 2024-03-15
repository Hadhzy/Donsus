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

auto DonsusSema::donsus_typecheck_is_compatible() -> bool {}