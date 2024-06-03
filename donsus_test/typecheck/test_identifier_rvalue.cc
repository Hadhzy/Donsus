

#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(IdentifierRvalueCheckCorrect, IdentifierRvalueCheck) {
  std::string a = R"(
        b:int = 11;
        a:int = b;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

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

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(IdentifierRvalueCheckInCorrectUndefined, IdentifierRvalueCheck) {
  std::string a = R"(
        # b:int = 11; -> except undefined here

         a:int = b;
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

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

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      DonsusUndefinedException);
}