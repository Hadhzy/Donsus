#ifndef DONSUS_SEMA_H
#define DONSUS_SEMA_H
#include "../Include/Internal/type.h"
#include "../Include/symbol_table.h"
#include "../src/utility/handle.h"
#include "../src/utility/memory_alloc.h"
#include "parser.h"
#include "symbol_table.h"
#include <set>

auto assign_type_to_node(utility::handle<donsus_ast::node> node,
                         utility::handle<DonsusSymTable> table,
                         utility::handle<DonsusSymTable> global_table) -> void;
void donsus_sym(utility::handle<donsus_ast::node> node,

                utility::handle<DonsusSymTable> table,
                utility::handle<DonsusSymTable> global_table);

class DonsusSema {
public:
  DonsusSema() = default;
  explicit DonsusSema(DonsusAstFile &file,
                      utility::handle<donsus_ast::tree> tree);
  using end_result = DonsusParser::end_result;
  using donsus_type = DONSUS_TYPE::kind;

  void start_traverse(utility::handle<DonsusSymTable>,
                      DonsusCodegen::DonsusCodeGenerator &codegen);

  void start_traverse(utility::handle<DonsusSymTable>);

  auto process_donsus_expression(utility::handle<donsus_ast::node> node,
                                 utility::handle<DonsusSymTable> table,
                                 utility::handle<DonsusSymTable> global_table)
      -> void;
  auto decide_type_for_expression(utility::handle<donsus_ast::node> node,
                                  utility::handle<DonsusSymTable> table)
      -> DONSUS_TYPE;
  // BASIC SEMA
  // true if its duplicated
  auto donsus_sema_is_duplicated(std::string &name,
                                 utility::handle<DonsusSymTable> table,
                                 bool for_function) -> bool;
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
  auto donsus_typecheck_is_return_type_valid_for_children(
      utility::handle<donsus_ast::node> node, std::vector<DONSUS_TYPE> expect,
      int &found) -> void;

  // ERRORS
  auto donsus_type_error(utility::handle<donsus_ast::node> node,
                         const std::string &message) -> void;

  auto donsus_type_error(const std::string &message) -> void;

  auto donsus_show_type_error_on_line(utility::handle<donsus_ast::node> node);
  auto donsus_make_pos_from_token(donsus_token &token) -> donsus_token_pos;

  // TRAVERSE
  auto assign_type_to_node(utility::handle<donsus_ast::node> node,
                           utility::handle<DonsusSymTable> table,
                           utility::handle<DonsusSymTable> global_table)
      -> void;

  auto donsus_sym(utility::handle<donsus_ast::node> node,
                  utility::handle<DonsusSymTable> table,
                  utility::handle<DonsusSymTable> global_table) -> void;

private:
  DonsusParserError error;
  DonsusAstFile *file;
  utility::handle<donsus_ast::tree> tree;
};
#endif // DONSUS_SEMA_H