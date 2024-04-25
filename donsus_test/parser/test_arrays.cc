#include "parser.h"
#include <gtest/gtest.h>

#include <iostream>

TEST(ArrayStructureTest, ArrayDefinitionTest) {
  std::string a = R"(
  a:int[] = [1, 2, 3];
)";
  DonsusParser::end_result result = Du_Parse(a);
  // match array_def
  auto array_def = result->get_nodes()[0]->type;
  EXPECT_EQ(array_def.type,
            donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION);

  // match elements
  auto elements = result->get_nodes()[0]->get<donsus_ast::array_def>().elements;
  EXPECT_EQ(elements.size(), 3);
  EXPECT_EQ(elements[0]->type.type,
            donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION);
}

TEST(ArrayDeclaration, ArrayDeclarationTest) {
  std::string a = R"(
  a:int[];
)";

  DonsusParser::end_result result = Du_Parse(a);
  // match array_decl
  auto array_decl = result->get_nodes()[0]->type;
  EXPECT_EQ(array_decl.type,
            donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION);
}