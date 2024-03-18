#include "../Include/parser.h"
#include <gtest/gtest.h>

#include <iostream>

/** \brief Check for return statement's node type
 * */
TEST(ReturnStatementTest, ReturnStatementNodeType) {
  std::string a = R"(
        def a() -> void{
          return 1;
      }
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT, type);
}

/** \brief Check for return statement's type
 * */
TEST(ReturnStatementTest, ReturnStatementReturnTypeNumber) {
  std::string a = R"(
        def a() -> int{
          return 1 + 2;
      }
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->children[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_EXPRESSION, type);
}

/** \brief Check for return statement's type
 * */
TEST(ReturnStatementTest, ReturnStatementReturnTypeIdentifier) {
  std::string a = R"(
        def a() -> int{
          return a;
      }
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->children[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_IDENTIFIER, type);
}

/** \brief Check for return statement's type
 * */
TEST(ReturnStatementTest, ReturnStatementReturnTypeIdentifierNumber) {
  std::string a = R"(
        def a() -> int{
          return a + 1;
      }
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->children[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_EXPRESSION, type);
}