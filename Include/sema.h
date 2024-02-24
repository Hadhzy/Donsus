#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "../Include/symbol_table.h"
#include "../src/utility/handle.h"
#include "../src/utility/memory_alloc.h"
#include "parser.h"

void donsus_sym(utility::handle<donsus_ast::node> node,
                utility::handle<DonsusSymTable> table);

/*
DonsusParser::end_result donsus_sema(DonsusParser::end_result ast,
                                     std::string &file_name);
*/

/*
utility::handle<donsus_symtable> donsus_sym(std::string &file_name);
*/

class DonsusSema {

public:
  DonsusSema() = default;
  using end_result = DonsusParser::end_result;
  auto donsus_sema(DonsusParser::end_result ast, std::string &file_name)
      -> DonsusSema::end_result;

private:
  bool error;
};
#endif // DONSUS_SEMA_H