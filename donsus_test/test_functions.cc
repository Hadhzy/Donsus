#include "../Include/parser.h"
#include <gtest/gtest.h>
#include <iostream>

// Test out function definition and declaration
TEST(Functions, FunctionDeclarationNodeType) {
  std::string a = R"(
        a() -> int;
    )";

  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->type.type;
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL, type);
}

TEST(Functions, FunctionDeclarationName) {
  std::string a = R"(
        a() -> int;
    )";

  DonsusParser::end_result result = Du_Parse(a);
  std::string name =
      result->get_nodes()[0]->get<donsus_ast::function_decl>().func_name;
  EXPECT_EQ("a", name);
}

TEST(Functions, FunctionDeclarationReturnType) {
  std::string a = R"(
        a() -> int;
    )";
  DonsusParser::end_result result = Du_Parse(a);
  DONSUS_TYPE::kind fn_return_type = result->get_nodes()[0]
                                         ->get<donsus_ast::function_decl>()
                                         .return_type[0]
                                         .type_un;
  EXPECT_EQ(DONSUS_TYPE::TYPE_BASIC_INT, fn_return_type);
}

TEST(Functions, FunctionDeclarationParameters) {
  std::string a = R"(
        a(b:int) -> int;
    )";
  DonsusParser::end_result result = Du_Parse(a);
  NAME_DATA_PAIR fn_param =
      result->get_nodes()[0]->get<donsus_ast::function_decl>().parameters[0];

  EXPECT_EQ(DONSUS_TYPE::TYPE_BASIC_INT, fn_param.type.type_un);
  EXPECT_EQ("b", fn_param.identifier);
}

TEST(Functions, FunctionDefinitionNodeType) {
  std::string a = R"(
        def a() -> int {};
    )";

  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]->type.type;
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF, type);
}

TEST(Functions, FunctionDefinitionNestedFunctions) {
  std::string a = R"(
        def a() -> int {
            def b() -> int {};
        };
    )";

  DonsusParser::end_result result = Du_Parse(a);
  donsus_ast::donsus_node_type::underlying type =
      result->get_nodes()[0]
          ->get<donsus_ast::function_def>()
          .body[0]
          ->type.type;
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF, type);
}

// the function's body will be tested with other tests relating to things that
// can be defined inside of the function's body