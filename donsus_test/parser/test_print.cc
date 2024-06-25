#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>

/*
 Single print on a DONSUS_EXPRESSION containing STRING_EXPRESSION.
 * */
TEST(TestPrintLiteral, TestPrintSystem) {
  std::string a = R"(
        printf("a");
    )";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION,
            result->get_nodes()[0]->children[0]->type.type);
  EXPECT_EQ(file.error_count, 0);
}