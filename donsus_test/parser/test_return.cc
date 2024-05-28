#include "parser.h"
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
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

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
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

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
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

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
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->children[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_EXPRESSION, type);
}
/** \brief Test for multiple return statement
 * *//*

TEST(MultipleReturnStatement, ReturnStatementMultipleWithScalars) {
  std::string a = R"(
    def a() -> int, int {
      return 3, 1;
      )";

  DonsusParser::end_result result = Du_Parse(a);
  std::vector<DONSUS_TYPE> expect =
      result->get_nodes()[0]->get<donsus_ast::function_def>().return_type;

  std::cout << expect[0].type_un;
  std::cout << expect[1].type_un;

  EXPECT_EQ(DONSUS_TYPE::TYPE_BASIC_INT, expect[0].type_un);
  EXPECT_EQ(DONSUS_TYPE::TYPE_BASIC_INT, expect[1].type_un);

  std::vector<utility::handle<donsus_ast::node>> nodes = result->get_nodes();

  for (auto node : nodes[0]->get<donsus_ast::function_def>().body) {
    if (node->type.type ==
        donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT) {
      for (auto type : expect) {
        for (auto type2 : node->get<donsus_ast::return_kw>().types) {
          EXPECT_EQ(type, type2);
        }
      }
      EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
                node->children[0]->type.type);
      EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
                node->children[1]->type.type);
    }
  }
}*/
