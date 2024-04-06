#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(UnaryExpressionsIncorrect, UnaryExpressionsTypecheck) {
  std::string a = R"(
    b:string = "12";
    a:int = -b;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(UnaryExpressionsIncorrect, UnaryExpressionsTypecheckUnsigned) {
  std::string a = R"(
    a:u32 = -1;
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      UnsignedTypeException);
}
