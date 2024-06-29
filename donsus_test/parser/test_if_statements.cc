#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(IfStatements, IfStatementsNodeType) {
  std::string a = R"(
        def a() -> int {
            if(1) {}
        };
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->type.type;
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT, type);
  EXPECT_EQ(file.error_count, 0);
}
TEST(IfStatements, IfStatementsGlobal) {
  std::string a = R"(
    if(1) {}
    )";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT, type);
  EXPECT_EQ(file.error_count, 0);
}

TEST(IfStatements, IfStatementsCondition) {
  std::string a = R"(
        def a() -> int {
            if(a > 10) {}
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

  donsus_token expression = result->get_nodes()[0]
                                ->get<donsus_ast::function_def>()
                                .body[0]
                                ->children[0]
                                ->get<donsus_ast::expression>()
                                .value;

  EXPECT_EQ(DONSUS_GREATER, expression.kind);
  EXPECT_EQ(">", expression.value);
  EXPECT_EQ(1, expression.length);
  EXPECT_EQ(1, expression.precedence);
  EXPECT_EQ(file.error_count, 0);
}

TEST(IfStatements, IfStatementsAlternateCase) {
  std::string a = R"(
        def a() -> int {
            if(a > 10) {

            } else {

            }
        }
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->get<donsus_ast::if_statement>()
          .alternate[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT, type);
  EXPECT_EQ(file.error_count, 0);
}

TEST(IfStatements, IfStatementsElifCase) {
  std::string a = R"(
        def a() -> int {
            if(a > 10) {

            } elif(a < 10) {

            }
        }
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->get<donsus_ast::if_statement>()
          .alternate[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT, type);
  EXPECT_EQ(file.error_count, 0);
}
