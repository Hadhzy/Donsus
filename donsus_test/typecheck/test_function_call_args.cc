#include "../Include/sema.h"
#include "../src/ast/tree.h"
#include <gtest/gtest.h>

// TEST(FunctionCallCheckCorrect, FunctionCallCheck) {
//   std::string a = R"(
//         def a(a:int, b:int) -> int {
//             return 1;
//         }

//     def b(c:int, d: string) -> int {
//         return 1;
//     }

//     b(1, "2");
//     )";

//   DonsusParser::end_result parse_result = Du_Parse(a);
//   utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
//   parse_result->init_traverse();
//   EXPECT_NO_THROW(
//       { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global);
//       });
// }

// TEST(FunctionCallCheckIncorrect, FunctionCallCheck) {
//   std::string a = R"(
//         def a(a:int, b:int) -> int {
//             return 1;
//         }

//     def b(c:int, d: string) -> int {
//         return 1;
//     }

//     b(1, 2);
//     )";

//   DonsusParser::end_result parse_result = Du_Parse(a);
//   utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();
//   parse_result->init_traverse();
//   EXPECT_THROW(
//       { parse_result->traverse(donsus_sym, assign_type_to_node, sym_global);
//       }, InCompatibleTypeException);
// }