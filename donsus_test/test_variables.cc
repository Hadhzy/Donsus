#include <gtest/gtest.h>
#include "../Include/parser.h"
#include "../Include/file.h"

#include <iostream>

TEST(VariableTest, VariableDefinition) {
  std::string a = R"(
    def f() -> int {
        a:int = 32;
      }
)";

  DonsusParser::end_result result = Du_Parse(a);


}