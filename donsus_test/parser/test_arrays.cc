#include "parser.h"
#include <gtest/gtest.h>

#include <iostream>

TEST(ArrayStructureTest, ArrayDefinitionTest) {
  std::string a = R"(
  a:int[] = [1, 2, 3];
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  // match array_def
  auto array_def = result->get_nodes()[0]->type;
  EXPECT_EQ(array_def.type,
            donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION);

  auto elements = result->get_nodes()[0]->get<donsus_ast::array_def>().elements;
  EXPECT_EQ(elements.size(), 3);
  EXPECT_EQ(elements[0]->type.type,
            donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION);
  EXPECT_EQ(file.error_count, 0);
}

TEST(ArrayDeclaration, ArrayDeclarationTest) {
  std::string a = R"(
  a:int[];
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  auto array_decl = result->get_nodes()[0]->type;
  EXPECT_EQ(array_decl.type,
            donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION);
}

TEST(ArrayAccess, TestArrays) {
  std::string a = R"(
  a:int[] = [0];

  b:int = a[0];
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  // match array_decl
  auto array_access =
      result->get_nodes()[1]->children[0]->get<donsus_ast::array_access>();
  EXPECT_EQ(result->get_nodes()[1]->children[0]->type.type,
            donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS);
  EXPECT_EQ(array_access.identifier_name, "a");
  EXPECT_EQ(file.error_count, 0);
}

TEST(ArrayAccessExpressionAsIndex, TestArrays) {
  std::string a = R"(
  a:int[] = [0];

  b:int = a[0 + 0];
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  // match array_decl
  auto array_access =
      result->get_nodes()[1]->children[0]->get<donsus_ast::array_access>();
  EXPECT_EQ(result->get_nodes()[1]->children[0]->type.type,
            donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS);
  EXPECT_EQ(array_access.identifier_name, "a");
  EXPECT_EQ(array_access.index->type.type,
            donsus_ast::donsus_node_type::DONSUS_EXPRESSION);
  EXPECT_EQ(file.error_count, 0);
}