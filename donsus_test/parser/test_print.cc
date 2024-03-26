#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>

/*
 Single print on a DONSUS_EXPRESSION containing STRING_EXPRESSION.
 * */
TEST(TestPrintLiteral, TestPrintSystem) {
  std::string a = R"(
        print("a");
    )";
  DonsusParser::end_result result = Du_Parse(a);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION, result->get_nodes()[0]->children[0]->type.type);
}