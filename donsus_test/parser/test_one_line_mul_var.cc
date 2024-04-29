#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(OneLineMulVar, OneLineMulVarDeclTest) {
  std::string a = R"(
        a,b,c,d:int;
    )";

  DonsusParser::end_result result = Du_Parse(a);
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

  DonsusParser::end_result result = Du_Parse(a);
  EXPECT_EQ(4, result->get_nodes().size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[0]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[1]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[2]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION,
            result->get_nodes()[3]->type.type);
  EXPECT_EQ("12", result->get_nodes()[0]
                      ->children[0]
                      ->get<dosnus_ast::number_expr>()
                      .value.value);
    EXPECT_EQ("12", result->get_nodes()[1]
    //                       ->children[0]
    //                       ->get<dosnus_ast::number_expr>()
    //                       .value.value);
    //   EXPECT_EQ("12", result->get_nodes()[2]
    //                       ->children[0]
    //                       ->get<dosnus_ast::number_expr>()
    //                       .value.value);
    //   EXPECT_EQ("12", result->get_nodes()[3]
    //                       ->children[0]
    //                       ->get<dosnus_ast::number_expr>()
    //                       .value.value);
}