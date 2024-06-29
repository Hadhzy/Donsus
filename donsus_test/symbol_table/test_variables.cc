#include "../Include/Internal/type.h"
#include "../Include/sema.h"
#include "../Include/symbol_table.h"
#include <gtest/gtest.h>
#include "../src/ast/tree.h"

/*
 This checks whether a simple variable is correctly added to the symbol_table
 * */
TEST(SymbolTableCheckScalar, SymbolTableVariable) {
  std::string a = R"(
    a: int = 12 + 7 / 2;
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  // expected symbol
  DonsusSymTable::sym ex_sym;
  DONSUS_TYPE type;

  type.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
  ex_sym.type = type;
  ex_sym.short_name = "a";
  ex_sym.key = "global.a";

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  DonsusSymTable::sym a_sym = sym_global->get("a");
  EXPECT_EQ(a_sym, ex_sym);
  EXPECT_EQ(file.error_count, 0);
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
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

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

  DonsusSema sema(file, parse_result);

  sema.start_traverse(sym_global);

  std::string function_global_table = "global.b";

  utility::handle<DonsusSymTable> a_symtable =
      sym_global->get_sym_table(function_global_table);
  EXPECT_EQ(a_symtable, table_a);
  EXPECT_EQ(file.error_count, 0);
}

/*

This checks whether a symbol is duplicated in a symbol-table.
 * */
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

/*
Test if the symbols have proper types.
As of now any number expression evaluates to a DONSUS_BASIC_INT
meaning all the other types must be compatible with them.
 */
TEST(SymbolType, SymbolTableVariable) {
  std::string a = R"(
    a: int = 12 + 7 / 2; # DONSUS_BASIC_INT
    b: int32 = 12 + 7; # DONSUS_I32
    c: u64 = 20 + 7; # DONSUS_U64
    d: int8 = 10 + 40; # DONSUS_I8
    e: int64 = 20; # DONSUS_I64
    f: int16 = 20; # DONSUS_I16
    g: u32 = 12; # DONSUS_U32
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  DonsusSymTable::sym sym_a = sym_global->get("a");
  DonsusSymTable::sym sym_b = sym_global->get("b");
  DonsusSymTable::sym sym_c = sym_global->get("c");
  DonsusSymTable::sym sym_d = sym_global->get("d");
  DonsusSymTable::sym sym_e = sym_global->get("e");
  DonsusSymTable::sym sym_f = sym_global->get("f");
  DonsusSymTable::sym sym_g = sym_global->get("g");

  EXPECT_EQ(sym_a.type.type_un, DONSUS_TYPE::TYPE_BASIC_INT);
  EXPECT_EQ(sym_b.type.type_un, DONSUS_TYPE::TYPE_I32);
  EXPECT_EQ(sym_c.type.type_un, DONSUS_TYPE::TYPE_U64);
  EXPECT_EQ(sym_d.type.type_un, DONSUS_TYPE::TYPE_I8);
  EXPECT_EQ(sym_e.type.type_un, DONSUS_TYPE::TYPE_I64);
  EXPECT_EQ(sym_f.type.type_un, DONSUS_TYPE::TYPE_I16);
  EXPECT_EQ(sym_g.type.type_un, DONSUS_TYPE::TYPE_U32);
  EXPECT_EQ(file.error_count, 0);
}

/*
 These types should not be equal
 * */
TEST(SymbolTypeNotEqual, SymbolTableVariable) {
  DONSUS_TYPE type1;
  DONSUS_TYPE type2;

  type1.type_un = DONSUS_TYPE::TYPE_I32;
  type2.type_un = DONSUS_TYPE::TYPE_U64;

  DonsusSymTable::sym symbol1;
  DonsusSymTable::sym symbol2;

  symbol1.type = type1;
  symbol2.type = type2;
  EXPECT_NE(symbol1, symbol2);
}
