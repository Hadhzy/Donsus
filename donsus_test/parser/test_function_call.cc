#include "parser.h"
#include <gtest/gtest.h>

#include <iostream>

/** \brief Check for function's name
 * */
TEST(FunctionCallTest, FunctionName) {
  std::string a = R"(
    def a() -> int {
      return 1;
    }
    
    a();
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();


  std::string _func_name =
      result->get_nodes()[1]->get<donsus_ast::function_call>().func_name;

  std::cout << _func_name << std::endl;

  EXPECT_EQ("a", _func_name);
}

/** \brief Check for function's arguments(just positional as of now)
 * */
TEST(FunctionCallTest, FunctionArguments) {
  std::string a = R"(
        func_name(1, "2", test());
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();


  std::vector<utility::handle<donsus_ast::node>> arguments =
      result->get_nodes()[0]->get<donsus_ast::function_call>().arguments;

  utility::handle<donsus_ast::node> first = arguments[0];
  utility::handle<donsus_ast::node> second = arguments[1];
  utility::handle<donsus_ast::node> third = arguments[2];

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
            first->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION,
            second->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL,
            third->type.type);
}
