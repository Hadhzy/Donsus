#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

TEST(ArrayTestTypecheckCorrect, ArrayTypeCheck) {
  std::string a = R"(
    a:int[] = [1, 2, 3];
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(ArrayTestTypecheckCorrect1, ArrayTypeCheck) {
  std::string a = R"(
    b:int = 12;
    a:int[] = [1, 2, b];
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}

TEST(ArrayTestTypecheckincorrect, ArrayTypeCheck) {
  std::string a = R"(
    b:string = "12";
    a:int[] = [1, 2, b];
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(ArrayTestTypecheckincorrect1, ArrayTypeCheck) {
  std::string a = R"(
    a:int[] = [1, 2, "12"];
)";

  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}
TEST(ArrayOutOfBounds, ArrayTypeCheck) {
  std::string a = R"(
    a:int[1] = [1, 2];
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      OutOfBoundException);
}

TEST(ArrayAccessIncorrect, ArrayTypeCheck) {
  std::string a = R"(
    a:string[] = ["string"];

    b:int = a[0];
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); },
      InCompatibleTypeException);
}

TEST(ArrayAccessCorrect, ArrayTypeCheck) {
  std::string a = R"(
    a:int[] = [0];

    b:int = a[0];
)";
  DonsusParser::end_result parse_result = Du_Parse(a);
  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
  parse_result->init_traverse();

  EXPECT_NO_THROW(
      { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global); });
}