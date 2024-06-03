#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(FunctionCallCheckCorrect, FunctionCallCheck) {
  std::string a = R"(
        def a(a:int, b:int) -> int {
            return 1;
        }

    def b(c:int, d: string) -> int {
        return 1;
    }

    b(1, "2");
    )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result parse_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(FunctionCallCheckIncorrect, FunctionCallCheck) {
  std::string a = R"(
        def a(a:int, b:int) -> int {
            return 1;
        }

    def b(c:int, d: string) -> int {
        return 1;
    }

    b(1, 2);
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