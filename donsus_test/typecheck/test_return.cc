#include "../../Include/codegen/codegen.h"
#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>
/*

 * */
TEST(ReturnStatementTypeCheckScalarCorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->int{
    return 1;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(ReturnStatementTypeCheckIdentifierCorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    b:int = 12;
    def a() ->int {
    return b;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(ReturnStatementTypeCheckScalarIncorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->string{
    return 1;
}
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReturnTypeException);
}

TEST(ReturnStatementTypeCheckIdentifierIncorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    b:int = 12;
    def a() ->string{
    return b;
}
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReturnTypeException);
}

TEST(ReturnStatementTypeCheckScalarIncorrectWithMultiple,
     ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->int, string{
    return 1;
}
)";

  DonsusParser::end_result parse_result = Du_Parse(a);

  parse_result->init_traverse();
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReturnTypeException);
}