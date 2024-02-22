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
#include "../Include/parser.h"
#include "../src/ast/node.h"
// Make symbol table
void donsus_sym(utility::handle<donsus_ast::node> node,
                utility::handle<DonsusSymTable> table) {
  // Construct sym table from assembly
/*  switch (node->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    table->add(node.>().identifier_name);
  }
  default: {
  }
  }*/
}
/*
utility::handle<DonsusSymTable> donsus_sym(){
  DonsusSymTable* global =
}
*/

/*
 *
DonsusParser::end_result donsus_sema(DonsusParser::end_result ast,
                                     std::string &file_name) {
  // TBD:
  // make sema
  // donsus_sym(file_name); // ahh, might use it here instead of runner.cc
  return ast;
}
*/
