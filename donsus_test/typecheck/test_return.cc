#include "../../Include/codegen/codegen.h"
#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>
/*

 * */
TEST(ReturnStatementTypeCheckScalarCorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->int{
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

TEST(ReturnStatementTypeCheckIdentifierCorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    b:int = 12;
    def a() ->int {
    return b;
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

TEST(ReturnStatementTypeCheckIdentifierCorrect2, ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->int {
    b:int = 12;
    return b;
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

TEST(ReturnStatementTypeCheckScalarIncorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->string{
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

TEST(ReturnStatementTypeCheckIdentifierIncorrect, ReturnStatementTypecheck) {
  std::string a = R"(
    b:int = 12;
    def a() ->string{
    return b;
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

TEST(ReturnStatementTypeCheckScalarIncorrectWithMultiple,
     ReturnStatementTypecheck) {
  std::string a = R"(
    def a() ->int, string{
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