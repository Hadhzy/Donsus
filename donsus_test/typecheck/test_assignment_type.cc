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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_EQ(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_EQ(file.error_count, 0);
}

TEST(AssignmentTypeCheckTypeCustomInCorrect, AssignmentTypeCheck) {
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
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


  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


    EXPECT_NE(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
}