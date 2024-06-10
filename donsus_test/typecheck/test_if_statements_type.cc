#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(IfStatementTypecheckCorrect, IfStatementTypecheck) {
  std::string a = R"(
    def a() ->int {
   if(1) {
      b:int = 12;
   }
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

TEST(IfStatementTypecheckInCorrect, IfStatementTypecheck) {
  std::string a = R"(
    def a() ->int {
   if(1) {
      b:string = 12;
   }
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