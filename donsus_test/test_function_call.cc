#include "../Include/parser.h"
#include <gtest/gtest.h>

#include <iostream>

/** \brief Check for function's name
 * */
TEST(FunctionCallTest, FunctionName) {
  std::string a = R"(
        func_name();
)";
  DonsusParser::end_result result = Du_Parse(a);

  std::string func_name =
      result->get_nodes()[0]->get<donsus_ast::function_call>().func_name;

  EXPECT_EQ("func_name", func_name);
}

/** \brief Check for function's arguments(just positional as of now)
 * */
TEST(FunctionCallTest, FunctionArguments) {
  std::string a = R"(
        func_name(1, 2, 3);
)";
  DonsusParser::end_result result = Du_Parse(a);

  std::vector<NAME_OR_DATA_PAIR> arguments =
      result->get_nodes()[0]->get<donsus_ast::function_call>().arguments;

  std::string first = arguments[0].identifier;
  std::string second = arguments[1].identifier;
  std::string third = arguments[2].identifier;

  EXPECT_EQ("1", first);
  EXPECT_EQ("2", second);
  EXPECT_EQ("3", third);
}
