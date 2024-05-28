#include "parser.h"
#include <gtest/gtest.h>

#include <iostream>
// we don't write tests for typechecking as it
// is quite undecidable whether we want to have implicit conversion
// for constants

TEST(FloatTest, FloatExpression){
  std::string a = R"(
        a:f32 = 32.3;
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->children[0]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION, type);
}


TEST(FloatTest, FloatExpressionf64){
  std::string a = R"(
        a:f64 = 32.3;
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->children[0]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION, type);
}


TEST(FloatTest, FloatExpressionf64Add){
  std::string a = R"(
        a:f64 = 32.3 + 34.3;
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->children[0]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_EXPRESSION, type);
}


