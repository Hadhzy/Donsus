

#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(FunctionCallRvalueCheckCorrect, FunctionCallRvalueCheck) {
  std::string a = R"(
        def function_call() -> int {
    return 1;
};

a:int = function_call();
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);


  EXPECT_EQ(file.error_count, 0);
}

TEST(FunctionCallRvalueCheckInCorrect, FunctionCallRvalueCheck) {
  std::string a = R"(
        def function_call() -> string {
    return 1;
};

a:int = function_call();
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusSema sema(file, parse_result);
  sema.start_traverse(sym_global);

  EXPECT_NE(file.error_count, 0);
}