#include "../Include/sema.h"
#include <gtest/gtest.h>

TEST(ForLoopIncorrect, ForLoopCheck) {
  std::string a = R"(
    not_my_array: int[2] = [1, 2];

    for number: my_array {
        printf("We got a : ", number);
    }
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      DonsusUndefinedException);
}

TEST(ForLoopIncorrect1, ForLoopCheck) {
  std::string a = R"(
    not_my_array: int[2] = [1, 2];

    for number: my_array {
        printf("We got a : ", number);
    }
    )";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();
  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      DonsusUndefinedException);
}