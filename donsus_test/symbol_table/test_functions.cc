#include "../Include/Internal/type.h"
#include "../Include/sema.h"
#include "../Include/symbol_table.h"
#include <gtest/gtest.h>

/*
 Check for nested functions and examine the child-parent behaviour between
 the symbol tables
 * */
TEST(SymbolTableCheckNestedFunctions, SymbolTableCheckFunction) {
  std::string a = R"(
    def func() -> int{
      def a() -> int {
        return 5;
        }
    return 5;
    }
)";

  DonsusParser::end_result parse_result = Du_Parse(a);

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  utility::handle<DonsusSymTable> sym_global2 = new DonsusSymTable();

  utility::handle<DonsusSymTable> nested_1 = sym_global2->add_sym_table("func");
  utility::handle<DonsusSymTable> c = nested_1->add_sym_table("a");

  parse_result->init_traverse();
  parse_result->traverse(donsus_sym, assign_type_to_node, sym_global);
  EXPECT_EQ(sym_global, sym_global2);
}

/*
 Check for nested functions and examine the child-parent behaviour between
 the symbol tables
 * */
TEST(SymbolTableCheckDifferentTableStructures, SymbolTableCheckFunction) {
  std::string a = R"(
    def func() -> int{
    return 5;
    }

  a() ->int;
)";

  DonsusParser::end_result parse_result = Du_Parse(a);

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  utility::handle<DonsusSymTable> sym_global2 = new DonsusSymTable();

  utility::handle<DonsusSymTable> nested_1 = sym_global2->add_sym_table("func");
  DONSUS_TYPE type;
  type.type_un == DONSUS_TYPE::TYPE_BASIC_INT;

  std::string value =
      sym_global2->add("a", type); // I need to specify the type here

  parse_result->init_traverse();
  parse_result->traverse(donsus_sym, assign_type_to_node, sym_global);

  EXPECT_EQ(sym_global, sym_global2);
}

// Check for symbol type here in every cases
