#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(AssignmentTypeCheckTypeCorrect, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;

    a = 3 + 2;
    return 1;
      }
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
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
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(AssignmentTypeCheckType2Correct, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;
    b:char = "12";

    a = 3 + 2 + b;
    return 1;
      }
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
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
  DonsusParser::end_result parse_result = Du_Parse(a);
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
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(AssignmentTypeCheckTypeInCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a: char;
  c: int;

  a = 2 + 1;
  return 2;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
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
  c: char = "12";

  a = c;
  return 2;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType2InCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a: char;
  c: char = "12";
  b: char = "a";

  a = c + b + "abc" + 1;
  return 2;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType3InCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a: char;
  c: int = 12;
  b: char = "a";

  a = c + b + "abc" + "a";
  return 2;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(AssignmentTypeCheckType4InCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  a: char;
  def d() -> int{
  c: int = 12;
  b: char = "a";

  a = c + b + "abc" + "a";
  return 2;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}