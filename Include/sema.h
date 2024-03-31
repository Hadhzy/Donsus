#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "../Include/Internal/type.h"
#include "../Include/symbol_table.h"
#include "../src/utility/handle.h"
#include "../src/utility/memory_alloc.h"
#include "parser.h"
#include <set>

auto assign_type_to_node(utility::handle<donsus_ast::node> node,
                         utility::handle<DonsusSymTable> table,
                         utility::handle<DonsusSymTable> global_table) -> void;
void donsus_sym(utility::handle<donsus_ast::node> node,
                utility::handle<DonsusSymTable> table,
                utility::handle<DonsusSymTable> global_table);

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
  // true if its duplicated
  auto donsus_sema_is_duplicated(std::string &name,
                                 utility::handle<DonsusSymTable> table) -> bool;
  // true if its exist only once
  auto donsus_sema_is_exist(std::string &name,
                            utility::handle<DonsusSymTable> table) -> bool;
  auto donsus_is_function_exist(std::string &name,
                                utility::handle<DonsusSymTable> table) -> bool;
  // TYPECHECK
  auto donsus_typecheck_is_integer() -> bool;
  auto donsus_typecheck_is_compatible(DONSUS_TYPE first, DONSUS_TYPE second)
      -> bool;
  auto donsus_typecheck_type_is_bool_conversion(
      utility::handle<donsus_ast::node>) -> bool;
  auto donsus_typecheck_type_expr(utility::handle<donsus_ast::node> node)
      -> DONSUS_TYPE;
  void donsus_typecheck_support_between_types(
      utility::handle<donsus_ast::node> node,
      std::set<DONSUS_TYPE> *childTypes = nullptr);
  auto donsus_typecheck_is_valid_operator(donsus_token_kind kind) -> bool;
  auto
  donsus_typecheck_is_return_type_valid(utility::handle<donsus_ast::node> node)
      -> void;

private:
  bool error;
};
#endif // DONSUS_SEMA_H