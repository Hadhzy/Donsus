#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(VariableTypeCheckScalarCorrect, VariableTypecheck) {
  std::string a = R"(
    a:int = 12 + 7 / 2;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(VariableTypeCheckScalarInCorrect, VariableTypecheck) {
  std::string a = R"(
    a:string = 12 + 7 / 2;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(RedefinitionVariableInCorrect, RedefinitionVariableTypecheck) {
  std::string a = R"(
    a:int = 12 + 7 / 2;
    a:int = 7;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReDefinitionException);
}

TEST(RedeclarationVariableIncorrect, RedeclarationVariableTypecheck) {
  std::string a = R"(
    a:int;
    a:int;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReDefinitionException);
}

TEST(RedeclarationVariableIncorrectNotSameType,
     RedeclarationVariableTypecheck) {
  std::string a = R"(
    a:int;
    a:string;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReDefinitionException);
}

TEST(StringTypecheck, TestSingleTypes) {
  std::string a = R"(
    a:string = "s"; # right now this is correct, although this s not a string
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();

  std::string a2 = R"(
    a:int = "s"; # right now this is correct, although this s not a string
)";

  DonsusParser::end_result parse_result2 = Du_Parse(a2);
  utility::handle<DonsusSymTable> sym_global2 = new DonsusSymTable();

  parse_result2->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });

  EXPECT_THROW(
      {
        parse_result2->traverse(donsus_sym, assign_type_to_node, sym_global2);
      },
      InCompatibleTypeException);
}
