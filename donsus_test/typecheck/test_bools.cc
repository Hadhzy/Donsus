
#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(BooleanTypecheckAsExpressionsCorrect, BooleanTypecheck) {
  std::string a = R"(
        a:bool = false;
        b:bool = true;
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(BooleanTypecheckAsExpressionsInCorrect, BooleanTypecheck) {
  std::string a = R"(
        a:int = true;
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(BooleanTypecheckAsExpressionsInCorrect2, BooleanTypecheck) {
  std::string a = R"(
        a:bool = 12;
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}