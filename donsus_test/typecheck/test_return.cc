#include "../Include/sema.h"
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

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(ReturnStatementTypeCheckScalarIncorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->char{
    return 1;
}
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReturnTypeException);
}

TEST(ReturnStatementTypeCheckScalarIncorrectWithMultiple,
     ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->int, char{
    return 1;
}
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReturnTypeException);
}