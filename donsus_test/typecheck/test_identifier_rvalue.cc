

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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
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

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_EQ(file.error_count, 0);
}