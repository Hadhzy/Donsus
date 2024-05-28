#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(OneLineMulVar, OneLineMulVarDeclTest) {
  std::string a = R"(
        a,b,c,d:int;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  EXPECT_EQ(4, result->get_nodes().size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION,
            result->get_nodes()[0]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION,
            result->get_nodes()[1]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION,
            result->get_nodes()[2]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION,
            result->get_nodes()[3]->type.type);
}

TEST(OneLineMulVar, OneLineMulVarDefTest) {
  std::string a = R"(
        a,b,c,d:int = 12;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  EXPECT_EQ(4, result->get_nodes().size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[0]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[1]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[2]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[3]->type.type);
}