#include "../Include/parser.h"
#include <gtest/gtest.h>

#include <iostream>

/** \brief Check for assignment's name
 * */
TEST(AssignmentsTest, AssignmentName) {
  std::string a = R"(
  def a() -> void{
  a = a + 1;
    }
)";
  DonsusParser::end_result result = Du_Parse(a);
  std::string identifier_name = result->get_nodes()[0]
                                    ->get<donsus_ast::function_def>()
                                    .body[0]
                                    ->get<donsus_ast::assignment>()
                                    .identifier_name;

  EXPECT_EQ("a", identifier_name);
}

/** \brief Check for assignment's operator
 * */
TEST(AssignmentsTest, AssignmentOperator) {
  std::string a = R"(
  def a() -> void{
  a = a + 1;
    }
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_token op = result->get_nodes()[0]
                        ->get<donsus_ast::function_def>()
                        .body[0]
                        ->get<donsus_ast::assignment>()
                        .op;

  EXPECT_EQ(DONSUS_EQUAL, op.kind);
}

/** \brief Check for assignment value's type
 * */
TEST(AssignmentsTest, AssignmentValueType) {
  std::string a = R"(
  def a() -> void{
  a = a + 1;
    }
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying assignment_value_type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->children[0]
          ->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_EXPRESSION,
            assignment_value_type);
}