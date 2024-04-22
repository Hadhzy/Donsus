#include "parser.h"
#include <gtest/gtest.h>

#include <iostream>

/** \brief Check for variable definition node type
 * */
TEST(VariableTest, VariableDefinitionNodeType) {
  std::string a = R"(
        a:int = 32;
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION, type);
}

/** \brief Check for variable definition type
 * */
TEST(VariableTest, VariableDefinitionType) {
  std::string a = R"(
        a:int = 32;
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_token_kind type =
      result->get_nodes()[0]->get<donsus_ast::variable_decl>().identifier_type;

  EXPECT_EQ(DONSUS_BASIC_INT, type);
}

/** \brief Check for variable def identifier name
 * */
TEST(VariableTest, VariableDefinitionIdentifier) {
  std::string a = R"(
        a:int = 32;
)";
  DonsusParser::end_result result = Du_Parse(a);
  std::string name =
      result->get_nodes()[0]->get<donsus_ast::variable_decl>().identifier_name;

  EXPECT_EQ("a", name);
}

/** \brief Check for variable's value
 * */
TEST(VariableTest, VariableDefinitionValue) {
  std::string a = R"(
        a:int = 32;
)";
  DonsusParser::end_result result = Du_Parse(a);
  std::string value = result->get_nodes()[0]
                          ->children[0]
                          ->get<donsus_ast::number_expr>()
                          .value.value;

  EXPECT_EQ("32", value);
}

/** \brief Variable declaration
 * */

/** \brief Check for variable declaration node type
 **/
TEST(VariableTest, VariableDeclarationNodeType) {
  std::string a = R"(
        a:int;
)";
  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->type.type;

  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION, type);
}

/** \brief Check for variable declaration type
 **/
TEST(VariableTest, VariableDeclarationType) {
  std::string a = R"(
        a:int;
)";
  std::string function_a = R"(
    def a() ->void{
      a:int;
      }
  )";

  DonsusParser::end_result result = Du_Parse(a);
  DonsusParser::end_result function_result = Du_Parse(function_a);

  donsus_token_kind type =
      result->get_nodes()[0]->get<donsus_ast::variable_decl>().identifier_type;

  donsus_token_kind function_type = function_result->get_nodes()[0]
                                        ->get<donsus_ast::function_def>()
                                        .body[0]
                                        ->get<donsus_ast::variable_decl>()
                                        .identifier_type;

  EXPECT_EQ(DONSUS_BASIC_INT, type);
  EXPECT_EQ(DONSUS_BASIC_INT, function_type);
}

/** \brief Check for variable decl identifier name
 * */
TEST(VariableTest, VariableDeclarationIdentifier) {
  std::string a = R"(
        a:int;
)";
  DonsusParser::end_result result = Du_Parse(a);
  std::string name =
      result->get_nodes()[0]->get<donsus_ast::variable_decl>().identifier_name;

  EXPECT_EQ("a", name);
}
