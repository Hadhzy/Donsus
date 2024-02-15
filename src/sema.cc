// Construct symbol table from ast(1)
// Semantic analysis generates SEA OF NODES from symbol table(2) or AST
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
#include "../Include/symbol_table.h"

// Make symbol table
utility::handle<donsus_symtable> donsus_sym(std::string &file_name, utility::DonsusAllocator& alloc) {
  utility::handle<donsus_symtable> t;

  t = donsus_sym_make(file_name, alloc); // construct a symbol table GLOBAL
  return t;
}

DonsusParser::parse_result
donsus_sema(DonsusParser::parse_result ast, std::string &file_name) {
  // TBD:
  // make sema
  // donsus_sym(file_name); // ahh, might use it here instead of runner.cc
  return ast;
}
