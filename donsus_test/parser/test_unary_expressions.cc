#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(UnaryExpressionsRvalue, UnaryExpressionsCheck) {
  std::string a = R"(
        a:int = -5;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();


  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->children[0]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION, type);
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
            result->get_nodes()[0]->children[0]->children[0]->type.type);
  EXPECT_EQ("5", result->get_nodes()[0]
                     ->children[0]
                     ->children[0]
                     ->get<donsus_ast::number_expr>()
                     .value.value);
}
