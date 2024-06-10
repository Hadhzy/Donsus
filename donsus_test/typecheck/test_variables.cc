#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(VariableTypeCheckScalarCorrect, VariableTypecheck) {
  std::string a = R"(
    a:int = 12 + 7 / 2;
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
}

TEST(VariableTypeCheckScalarInCorrect, VariableTypecheck) {
  std::string a = R"(
    a:string = 12 + 7 / 2;
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}

TEST(RedefinitionVariableInCorrect, RedefinitionVariableTypecheck) {
  std::string a = R"(
    a:int = 12 + 7 / 2;
    a:int = 7;
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}

TEST(RedeclarationVariableIncorrect, RedeclarationVariableTypecheck) {
  std::string a = R"(
    a:int;
    a:int;
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}

TEST(RedeclarationVariableIncorrectNotSameType,
     RedeclarationVariableTypecheck) {
  std::string a = R"(
    a:int;
    a:string;
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}

TEST(StringTypecheck, TestSingleTypes) {
  std::string a = R"(
    a:string = "s"; # right now this is correct, although this s not a string
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  // Todo: not sure what's going on here
  std::string a2 = R"(
    a:int = "s"; # right now this is correct, although this s not a string
)";

  DonsusAstFile file2;
  DonsusParser parser2 = Du_Parse(a2, file2);
  DonsusParser::end_result parse_result2 = parser2.donsus_parse();

  utility::handle<DonsusSymTable> sym_global2 = new DonsusSymTable();

  DonsusSema sema2(file2, parse_result2);
  sema2.start_traverse(sym_global2);

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(file2.error_count, 0);
}
