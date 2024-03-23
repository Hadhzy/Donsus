#include "../Include/sema.h"
#include <gtest/gtest.h>
/*

* */

TEST(VariableTypeCheckScalarCorrect, VariableTypecheck) {
  std::string a = R"(
    a:int = 12 + 7 / 2;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(VariableTypeCheckScalarInCorrect, VariableTypecheck) {
  std::string a = R"(
    a:char = 12 + 7 / 2;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(RedefinitionVariableInCorrect, RedefinitionVariableTypecheck) {
  std::string a = R"(
    a:int = 12 + 7 / 2;
    a:int = 7;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReDefinitionException);
}

TEST(RedeclarationVariableIncorrect, RedeclarationVariableTypecheck) {
  std::string a = R"(
    a:int;
    a:int;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReDefinitionException);
}

TEST(RedeclarationVariableIncorrectNotSameType,
     RedeclarationVariableTypecheck) {
  std::string a = R"(
    a:int;
    a:char;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReDefinitionException);
}
