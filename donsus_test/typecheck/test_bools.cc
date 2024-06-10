
#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(BooleanTypecheckAsExpressionsCorrect, BooleanTypecheck) {
  std::string a = R"(
        a:bool = false;
        b:bool = true;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
}

TEST(BooleanTypecheckAsExpressionsInCorrect, BooleanTypecheck) {
  std::string a = R"(
        a:int = true;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
}

TEST(BooleanTypecheckAsExpressionsInCorrect2, BooleanTypecheck) {
  std::string a = R"(
        a:bool = 12;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);



  EXPECT_NE(file.error_count, 0);
}