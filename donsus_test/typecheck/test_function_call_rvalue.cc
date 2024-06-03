

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
  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
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
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      ReturnTypeException);
}