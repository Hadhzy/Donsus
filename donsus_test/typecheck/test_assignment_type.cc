#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(AssignmentTypeCheckTypeCorrect, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;

    a = 3 + 2;
    return 1;
      }
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(AssignmentTypeCheckType1Correct, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;
    b:int = 12;

    a = 3 + 2 + b;
    return 1;
      }
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(AssignmentTypeCheckType2Correct, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;
    b:string = "12";

    a = 3 + 2 + b;
    return 1;
      }
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType3Correct, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;
    c: int = 111;
    b:int = 12;

    a = b+c;
    return 1;
      }
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(AssignmentTypeCheckType4Correct, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;
    b:int = 12;

    a = b;
    return 1;
      }
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(AssignmentTypeCheckTypeInCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a: string;
  c: int;

  a = 2 + 1;
  return 2;
}
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType1InCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a:int;
  c: string = "12";

  a = c;
  return 2;
}
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType2InCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a: string;
  c: string = "12";
  b: string = "a";

  a = c + b + "abc" + 1;
  return 2;
}
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType3InCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a: string;
  c: int = 12;
  b: string = "a";

  a = c + b + "abc" + "a";
  return 2;
}
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType4InCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  a: string;
  def d() -> int{
  c: int = 12;
  b: string = "a";

  a = c + b + "abc" + "a";
  return 2;
}
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}