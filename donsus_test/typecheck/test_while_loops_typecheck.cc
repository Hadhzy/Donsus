#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(WhileLoopCorrect, WhileLoopTypeCheck) {
  std::string a = R"(
        a:int = 32;
        while a < 10 {
          a = a + 1;
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

TEST(WhileLoopCorrect2, WhileLoopTypeCheck) {
  std::string a = R"(
        a:int = 32;
        b:int = 40;
        while a < b {
          a = a + 1;
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

// Todo: finish codegen for while loop
/*TEST(WhileLoopIncorrect, WhileLoopTypeCheck) {
  std::string a = R"(
        a:int = 32;
        while a < "hello" {
          a = a + 1;
        }
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}*/

TEST(WhileLoopIncorrect2, WhileLoopTypeCheck) {
  std::string a = R"(
        a:int = 32;
        while a < b {
          a = a + 1;
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