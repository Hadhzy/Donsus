#include "../Include/sema.h"
#include <gtest/gtest.h>
#include "../src/ast/tree.h"

TEST(AssignmentTypeCheckTypeCorrect, AssignmentTypeCheck) {
  std::string a = R"(
    def d() ->int {
    a: int;

    a = 3 + 2;
    return 1;
      }
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(AssignmentTypeCheckTypeInCorrect, AssignmentTypeCheck) {
  std::string a = R"(
  def d() -> int{
  a: char;
  c: int;

  a = 2 + 1;
  return 2;
}
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}
