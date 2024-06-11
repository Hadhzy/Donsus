#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(ArrayTestTypecheckCorrect, ArrayTypeCheck) {
  std::string a = R"(
    a:int[] = [1, 2, 3];
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
}

TEST(ArrayTestTypecheckCorrect1, ArrayTypeCheck) {
  std::string a = R"(
    b:int = 12;
    a:int[] = [1, 2, b];
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_EQ(file.error_count, 0);
}

TEST(ArrayTestTypecheckincorrect, ArrayTypeCheck) {
  std::string a = R"(
    b:string = "12";
    a:int[] = [1, 2, b];
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
}

TEST(ArrayTestTypecheckincorrect1, ArrayTypeCheck) {
  std::string a = R"(
    a:int[] = [1, 2, "12"];
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
}
TEST(ArrayOutOfBounds, ArrayTypeCheck) {
  std::string a = R"(
    a:int[1] = [1, 2];
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
}

TEST(ArrayAccessIncorrect, ArrayTypeCheck) {
  std::string a = R"(
    a:string[] = ["string"];

    b:int = a[0];
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_NE(file.error_count, 0);
}

TEST(ArrayAccessCorrect, ArrayTypeCheck) {
  std::string a = R"(
    a:int[] = [0];

    b:int = a[0];
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
}