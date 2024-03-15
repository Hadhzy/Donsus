#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "../Include/Internal/type.h"
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
  using donsus_type = DONSUS_TYPE::kind;
  // BASIC SEMA
  auto donsus_sema(utility::handle<donsus_ast::node> ast) -> void;
  auto donsus_sema_is_defined(donsus_type ast, utility::handle<DonsusSymTable>)
      -> void;
  // TYPECHECK
  auto donsus_typecheck_is_integer() -> bool;
  auto donsus_typecheck_is_compatible() -> bool;

private:
  bool error;
};
#endif // DONSUS_SEMA_H