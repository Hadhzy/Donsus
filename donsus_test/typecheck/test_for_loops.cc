#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(ForLoopIncorrect, ForLoopCheck) {
  std::string a = R"(
    not_my_array: int[2] = [1, 2];

    for number: my_array {
        printf("We got a : ", number);
    }
    )";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parser_result = parser.donsus_parse();
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parser_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}

TEST(ForLoopIncorrect1, ForLoopCheck) {
  std::string a = R"(
    not_my_array: int[2] = [1, 2];

    for number: my_array {
        printf("We got a : ", number);
    }
    )";

  DonsusAstFile file;

  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parser_result = parser.donsus_parse();
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parser_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}