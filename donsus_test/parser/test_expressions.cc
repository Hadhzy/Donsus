#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(Expressions, ExpressionsNodeType) {
  std::string a = R"(
        a:int = (3/4) + (4/5);
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->children[0]->type.type;
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_EXPRESSION, type);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Expressions, ExpressionsValueNumbers) {
  std::string a = R"(
        a:int = (3/4) + (4/5);
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_token expression =
      result->get_nodes()[0]->children[0]->get<donsus_ast::expression>().value;
  EXPECT_EQ("+", expression.value);
  EXPECT_EQ(10, expression.precedence);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Expressions, ExpressionsBooleans) {
  std::string a = R"(
        a:bool = true;
        b:bool = false;
    )";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_token expression =
      result->get_nodes()[0]->children[0]->get<donsus_ast::bool_expr>().value;
  donsus_token expression1 =
      result->get_nodes()[1]->children[0]->get<donsus_ast::bool_expr>().value;
  EXPECT_EQ("true", expression.value);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION,
            result->get_nodes()[0]->children[0]->type.type);
  EXPECT_EQ("false", expression1.value);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION,
            result->get_nodes()[1]->children[0]->type.type);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Expressions, ExpressionsValueWithIdentifiers) {
  std::string a = R"(
        a:int = (3/a) + (a/5);
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_token expression =
      result->get_nodes()[0]->children[0]->get<donsus_ast::expression>().value;
  EXPECT_EQ("+", expression.value);
  EXPECT_EQ(10, expression.precedence);
  donsus_token expression_children = result->get_nodes()[0]
                                         ->children[0]
                                         ->children[0]
                                         ->get<donsus_ast::expression>()
                                         .value;

  EXPECT_EQ("/", expression_children.value);
  EXPECT_EQ(20, expression_children.precedence);

  donsus_token number_expression_children = result->get_nodes()[0]
                                                ->children[0]
                                                ->children[0]
                                                ->children[0]
                                                ->get<donsus_ast::expression>()
                                                .value;

  EXPECT_EQ("3", number_expression_children.value);
  EXPECT_EQ(0, number_expression_children.precedence);

  std::string identifier_expression_children =
      result->get_nodes()[0]
          ->children[0]
          ->children[0]
          ->children[1]
          ->get<donsus_ast::identifier>()
          .identifier_name;

  EXPECT_EQ("a", identifier_expression_children);
  EXPECT_EQ(file.error_count, 0);
}