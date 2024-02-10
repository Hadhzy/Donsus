#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "parser.h"
#include "../Include/symbol_table.h"

std::unique_ptr<donsus_ast> donsus_sema(std::unique_ptr<donsus_ast> ast, std::string& file_name);
std::unique_ptr<donsus_symtable> donsus_sym(std::string& file_name);

#endif // DONSUS_SEMA_H