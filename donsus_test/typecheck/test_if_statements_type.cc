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

  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
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

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}