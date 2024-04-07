

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

         a:string = b;
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(IdentifierRvalueCheckInCorrectUndefined, IdentifierRvalueCheck) {
  std::string a = R"(
        # b:int = 11;

         a:int = b;
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      DonsusUndefinedException);
}

TEST(IdentifierRvalueCheckInCorrectUndefinedAssignments,
     IdentifierRvalueCheck) {
  std::string a = R"(
        def d() -> int {
          # b:int = 11;

         a:int;
         a = 1 + b;
         return 1;
        }
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      DonsusUndefinedException);
}