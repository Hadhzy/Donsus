#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "../Include/symbol_table.h"
#include "parser.h"

donsus_global_ast*
donsus_sema(donsus_global_ast* ast, std::string &file_name);
donsus_symtable* donsus_sym(std::string &file_name);

#endif // DONSUS_SEMA_H