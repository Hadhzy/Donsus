#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include "parser.h"
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
  utility::handle<donsus_ast::node> fn_param =
      result->get_nodes()[0]->get<donsus_ast::function_decl>().parameters[0];
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  result->init_traverse();
  result->traverse(donsus_sym, assign_type_to_node, sym_global);
  /*
    Here the reason why we check for DONSUS_BASIC_INT instead of
    DONSUS_TYPE::TYPE_BASIC_INT is because when it comes to function
    declarations we don't actually add the things to the symbol table, we will
    only do so when the function has been defined, so therefore when its just a
    function declaration we will not assign type to it because it will only
    makes sense to do so when the function has been defined.
    If this would have been a function definition then we would have assigned
    DONSUS_TYPE::TYPE_BASIC_INT. (this might change in the future)
  */
  EXPECT_EQ(donsus_token_kind::DONSUS_BASIC_INT,
            fn_param->get<donsus_ast::variable_decl>().identifier_type);
  EXPECT_EQ("b", fn_param->get<donsus_ast::variable_decl>().identifier_name);
}

TEST(Functions, FunctionDefinitionParameters) {
  std::string a = R"(
        def a(b:int) -> int {
          return 1;
        };
    )";
  DonsusParser::end_result result = Du_Parse(a);

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  result->init_traverse();
  result->traverse(donsus_sym, assign_type_to_node, sym_global);

  utility::handle<donsus_ast::node> fn_param =
      result->get_nodes()[0]->get<donsus_ast::function_decl>().parameters[0];
  /*
    Now here we can check for DONSUS_TYPE::TYPE_BASIC_INT because its a function
    definition
  */
  EXPECT_EQ(DONSUS_TYPE::TYPE_BASIC_INT, fn_param->real_type.type_un);
  EXPECT_EQ("b", fn_param->get<donsus_ast::variable_decl>().identifier_name);
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