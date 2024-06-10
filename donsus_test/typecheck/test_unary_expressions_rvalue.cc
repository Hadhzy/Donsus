#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(UnaryExpressionsIncorrect, UnaryExpressionsTypecheck) {
  std::string a = R"(
    b:string = "12";
    a:int = -b;
)";
  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}

TEST(UnaryExpressionsIncorrect, UnaryExpressionsTypecheckUnsigned) {
  std::string a = R"(
    a:u32 = -1;
)";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}
