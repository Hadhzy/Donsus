

#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(IdentifierRvalueCheckCorrect, IdentifierRvalueCheck) {
  std::string a = R"(
        b:int = 11;
        a:int = b;
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(IdentifierRvalueCheckInCorrect, IdentifierRvalueCheck) {
  std::string a = R"(
        b:int = 11;

         a:char = b;
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}