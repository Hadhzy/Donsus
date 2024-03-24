// #include "parser.h"
// #include <gtest/gtest.h>

// #include <iostream>

// /** \brief Check for function's name
//  * */
// TEST(FunctionCallTest, FunctionName) {
//   std::string a = R"(
//         func_name();
// )";
//   DonsusParser::end_result result = Du_Parse(a);

//   std::string func_name =
//       result->get_nodes()[0]->get<donsus_ast::function_call>().func_name;

//   EXPECT_EQ("func_name", func_name);
// }

// /** \brief Check for function's arguments(just positional as of now)
//  * */
// TEST(FunctionCallTest, FunctionArguments) {
//   std::string a = R"(
//         func_name(1, "2", test());
// )";
//   DonsusParser::end_result result = Du_Parse(a);

//   std::vector<utility::handle<donsus_ast::node>> arguments =
//       result->get_nodes()[0]->get<donsus_ast::function_call>().arguments;

//   utility::handle<donsus_ast::node> first = arguments[0];
//   utility::handle<donsus_ast::node> second = arguments[1];
//   utility::handle<donsus_ast::node> third = arguments[2];

//   EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
//             first->type.type);
//   EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION,
//             second->type.type);
//   EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL,
//             third->type.type);
// }

// TEST(FunctionCallTest, FunctionAsExpression) {
//   std::string a = R"(
//         a:int = return_number(a,"b",12/5 + 4) + 2 + a + "a";
// )";
//   DonsusParser::end_result result = Du_Parse(a);

//   std::vector<utility::handle<donsus_ast::node>> arguments =
//       result->get_nodes()[0]->children;

//   utility::handle<donsus_ast::node> first = arguments[0];
//   utility::handle<donsus_ast::node> second = arguments[1];
//   utility::handle<donsus_ast::node> third = arguments[2];
//   utility::handle<donsus_ast::node> fourth = arguments[3];

//   EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL,
//             first->type.type);
//   EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
//             second->type.type);
//   EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_IDENTIFIER,
//   third->type.type);
//   EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION,
//             fourth->type.type);

//   // EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_IDENTIFIER)
// }
