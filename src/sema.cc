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

std::unique_ptr<donsus_symtable> donsus_sym(std::string& file_name){
    std::unique_ptr<donsus_symtable> t;
    std::unique_ptr<donsus_symbol> s;

    t = donsus_sym_make(file_name); // construct a symbol table GLOBAL

}

void donsus_sema(std::unique_ptr<donsus_ast>, std::string& file_name){
    // TBD:
    // make sema
    /*donsus_sym(file_name);
    return ast;*/
}
