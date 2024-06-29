#include "parser.h"
#include <gtest/gtest.h>

#include <iostream>

TEST(WhileLoopTest, WhileLoopStructureTest) {
  std::string a = R"(
        a:int = 32;
        while a < 10 {
          a = a + 1;
        }
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[1]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_WHILE_LOOP, type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_EXPRESSION,
            result->get_nodes()[1]->children[0]->type.type);
  EXPECT_EQ(2, result->get_nodes()[1]->children[0]->children.size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_IDENTIFIER,
            result->get_nodes()[1]->children[0]->children[0]->type.type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
            result->get_nodes()[1]->children[0]->children[1]->type.type);
  EXPECT_EQ(file.error_count, 0);
}
