#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "../Include/symbol_table.h"
#include "../src/utility/handle.h"
#include "../src/utility/memory_alloc.h"
#include "parser.h"

DonsusParser::parse_result donsus_sema(DonsusParser::end_result ast,
                                       std::string &file_name);

utility::handle<donsus_symtable> donsus_sym(std::string &file_name);

#endif // DONSUS_SEMA_H