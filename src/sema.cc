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
#include "../Include/sema.h"
#include "../Include/parser.h"
#include "../src/ast/node.h"
// Make symbol table
void donsus_sym(utility::handle<donsus_ast::node> node,
                utility::handle<DonsusSymTable> table) {
  //Todo: Loop through top level nodes and then child nodes
  switch (node->type.type) { /*
   case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
     auto &expr = node->get<donsus_ast::variable_decl>();
     table->add(expr.identifier_name);
     break;
   }
   case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
     auto &expr = node->get<donsus_ast::variable_decl>();
     table->add(expr.identifier_name);
     break;
   }
   case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
     auto &expr = node->get<donsus_ast::function_decl>();
     table->add(expr.func_name);
     break;
   }
   case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
     auto &expr = node->get<donsus_ast::function_def>();
     table->add(expr.func_name);
     break;
   }*/
  default: {
  }
  }
}

DonsusSema::end_result DonsusSema::donsus_sema(DonsusParser::end_result ast,
                                               std::string &file_name) {
  return ast;
}
