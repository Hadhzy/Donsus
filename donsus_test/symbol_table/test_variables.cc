#include "../Include/Internal/type.h"
#include "../Include/sema.h"
#include "../Include/symbol_table.h"
#include <gtest/gtest.h>

TEST(SymbolTableCheckScalar, SymbolTableVariable) {
  std::string a = R"(
    a: int = 12 + 7 / 2;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  // expected symbol
  DonsusSymTable::sym ex_sym;
  DONSUS_TYPE type;

  type.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
  ex_sym.type = type;
  ex_sym.short_name = "a";
  ex_sym.key = "global.a";

  parse_result->init_traverse();
  parse_result->traverse(donsus_sym, assign_type_to_node, sym_global);

  DonsusSymTable::sym a_sym = sym_global->get("a");
  EXPECT_EQ(a_sym, ex_sym);
}

/*

This function checks whether a manually created type with
 * */
TEST(SymbolTableCheckScalarInFunction, SymbolTableVariable) {
  std::string a = R"(
  def b() -> int {
    a: int = 12 + 7 / 2;

    return 1;
    }
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  // expected table
  utility::handle<DonsusSymTable> table_a;
  table_a = sym_global->add_sym_table("b");

  // expected symbol
  DonsusSymTable::sym ex_sym;
  DONSUS_TYPE type;

  type.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
  ex_sym.type = type;
  ex_sym.short_name = "a";
  ex_sym.key = "global.b.a";

  parse_result->init_traverse();
  parse_result->traverse(donsus_sym, assign_type_to_node, sym_global);

  std::string function_global_table = "global.b";

  utility::handle<DonsusSymTable> a_symtable =
      sym_global->get_sym_table(function_global_table);
  EXPECT_EQ(a_symtable, table_a);
}


TEST(SymbolTableDuplicated, SymbolTableVariable) {
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  // expected symbol
  DonsusSymTable::sym ex_sym_1;
  DonsusSymTable::sym ex_sym_2;
  DONSUS_TYPE type;

  type.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
  ex_sym_1.type = type;
  ex_sym_1.short_name = "a";
  ex_sym_1.key = "global.a";

  ex_sym_2.type = type;
  ex_sym_2.short_name = "a";
  ex_sym_2.key = "global.a";

  sym_global->add_symbol(ex_sym_1);
  sym_global->add_symbol(ex_sym_2);

  DonsusSymTable::sym is_duplicated = sym_global->get("a");

  EXPECT_EQ(true, is_duplicated.duplicated);
}