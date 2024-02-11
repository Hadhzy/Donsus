#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "../Include/symbol_table.h"
#include "parser.h"

std::unique_ptr<donsus_global_ast>
donsus_sema(std::unique_ptr<donsus_global_ast> ast, std::string &file_name);
std::unique_ptr<donsus_symtable> donsus_sym(std::string &file_name);

#endif // DONSUS_SEMA_H