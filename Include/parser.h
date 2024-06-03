// Declarations for parser.cc
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <vector>

#include "../Include/parser_util.h"
#include "../Include/symbol_table.h"
#include "../src/ast/tree.h"
#include "../src/utility/exception.h"
#include "../src/utility/handle.h"

#include "token.h"
#ifndef DEBUG
#define DEBUG 1 // debug
#endif
class DonsusParser;

// Second-highest group in Donsus
struct DonsusAstFile {
  int id;
  int flags;

  std::string fullpath;
  std::string filename;
  std::string directory;

  std::string absolute_path;

  std::string extension;

  unsigned int error_count{};
  DonsusParser *parser;
};
extern std::map<std::string, donsus_token_kind> DONSUS_TYPES_LEXER;

/**
 *  \brief Lexer API
 */
struct donsus_lexer {
  donsus_lexer(std::string input)
      : string(input), cur_pos(0), cur_column(0), cur_line(1),
        cur_char(input[0]) {}
  donsus_lexer() = default;

  std::string string;
  char cur_char;
  unsigned cur_column;
  unsigned int cur_pos, cur_line;
};

donsus_token donsus_lexer_next(DonsusParser &parser); // forward reference

class DonsusParser {
public:
  using parse_result = utility::handle<donsus_ast::node>;
  using end_result = utility::handle<donsus_ast::tree>;

  DonsusParser(donsus_lexer &lexer, DonsusAstFile &file);

  // move to the next token
  donsus_token donsus_parser_next();
  // move to the next token and see if the provided type is correct
  void donsus_parser_except(donsus_token_kind type);
  void donsus_parser_except_current(donsus_token_kind type);

  DonsusParser &operator=(const DonsusParser &f) {
    if (this != &f) {
      cur_token = f.cur_token;
      lexer = f.lexer;
      donsus_tree = f.donsus_tree;
      allocator = f.allocator;
      error = f.error;
      file = f.file;
    }
    return *this;
  }
  /**
   *  \brief Parse down:
   *  function definitions:
   *      - statements
   *
   */
  auto donsus_parse() -> end_result;

  void print_token();
  donsus_token donsus_peek(int loop = 1);

  // create node
  // parsing expression
  auto donsus_expr(unsigned int ptp) -> parse_result;
  auto match_expressions(unsigned int ptp) -> parse_result;
  auto create_expression(donsus_ast::donsus_node_type type,
                         u_int64_t child_count) -> parse_result;
  auto make_new_expr_node(donsus_token prev_token, parse_result &left,
                          parse_result &right) -> parse_result;
  // parsing number expressions
  auto donsus_number_primary(donsus_ast::donsus_node_type type,
                             uint64_t child_count) -> parse_result;
  // parsing multiple var decl, def in one line
  auto donsus_variable_multi_decl_def() -> void;

  // parsing variable declaration
  auto donsus_variable_decl() -> parse_result;
  auto create_variable_declaration(donsus_ast::donsus_node_type type,
                                   u_int64_t child_count) -> parse_result;

  // parsing array definition
  auto donsus_array_definition(utility::handle<donsus_ast::node> &declaration,
                               donsus_ast::ArrayType array_type, int size)
      -> parse_result;
  auto create_array_definition(donsus_ast::donsus_node_type type,
                               u_int64_t child_count) -> parse_result;

  // parsing array declaration
  auto donsus_array_declaration(utility::handle<donsus_ast::node> &declaration,
                                donsus_ast::ArrayType array_type, int size)
      -> parse_result;
  auto create_array_declaration(donsus_ast::donsus_node_type type,
                                u_int64_t child_count) -> parse_result;

  // parsing variable definition
  auto donsus_variable_definition(parse_result &declaration) -> parse_result;

  auto create_number_expression(donsus_ast::donsus_node_type type,
                                u_int64_t child_count) -> parse_result;

  // while loop
  auto donsus_while_loop() -> parse_result;
  auto create_while_loop(donsus_ast::donsus_node_type type,
                         u_int64_t child_count) -> parse_result;

  // range for loop
  auto donsus_range_for_loop(bool is_range_with_name) -> parse_result;
  auto create_range_for_loop(donsus_ast::donsus_node_type type,
                             u_int64_t child_count) -> parse_result;

  // array for loop
  auto donsus_array_for_loop(bool is_with_name) -> parse_result;
  auto create_array_for_loop(donsus_ast::donsus_node_type type,
                             u_int64_t child_count) -> parse_result;

  // float
  auto donsus_float_number(donsus_ast::donsus_node_type type,
                           uint64_t child_count) -> parse_result;

  auto create_float_expression(donsus_ast::donsus_node_type type,
                               u_int64_t child_count) -> parse_result;
  // print expression
  auto donsus_print() -> parse_result;
  auto create_donsus_print(donsus_ast::donsus_node_type type,
                           u_int64_t child_count) -> parse_result;

  // Function declaration
  auto donsus_function_decl() -> parse_result;
  auto create_function_decl(donsus_ast::donsus_node_type type,
                            u_int64_t child_count) -> parse_result;

  // Array access
  auto donsus_array_access() -> parse_result;
  auto create_array_access(donsus_ast::donsus_node_type type,
                           u_int64_t child_count) -> parse_result;

  // parsing string expressions
  auto string_expression() -> parse_result;
  auto create_string_expression(donsus_ast::donsus_node_type type,
                                u_int64_t child_count) -> parse_result;

  // parsing boolean expressions
  auto bool_expression() -> parse_result;
  auto create_bool_expression(donsus_ast::donsus_node_type type,
                              u_int64_t child_count) -> parse_result;

  // parsing unary expressions
  auto unary_expression() -> parse_result;
  auto create_unary_expression(donsus_ast::donsus_node_type type,
                               u_int64_t child_count) -> parse_result;

  // Function definition
  auto donsus_function_definition() -> parse_result;
  auto create_function_definition(donsus_ast::donsus_node_type type,
                                  u_int64_t child_count) -> parse_result;

  auto donsus_statements() -> std::vector<parse_result>;

  auto donsus_function_signature()
      -> std::vector<utility::handle<donsus_ast::node>>;
  auto donsus_function_args() -> std::vector<utility::handle<donsus_ast::node>>;

  // If statements
  auto donsus_if_statement() -> parse_result;
  auto create_if_statement(donsus_ast::donsus_node_type type,
                           u_int64_t child_count) -> parse_result;

  auto donsus_else_statement() -> parse_result;
  auto create_else_statement(donsus_ast::donsus_node_type type,
                             u_int64_t child_count) -> parse_result;

  // assignments
  auto donsus_assignments() -> parse_result;
  auto create_assignments(donsus_ast::donsus_node_type type,
                          u_int64_t child_count) -> parse_result;

  // identifier
  auto donsus_identifier() -> parse_result;
  auto create_identifier(donsus_ast::donsus_node_type type,
                         u_int64_t child_count) -> parse_result;

  auto donsus_function_call(donsus_token &name) -> parse_result;
  auto create_function_call(donsus_ast::donsus_node_type type,
                            u_int64_t child_count) -> parse_result;

  // return statement
  auto donsus_return_statement() -> parse_result;
  auto create_return_statement(donsus_ast::donsus_node_type type,
                               u_int64_t child_count) -> parse_result;

  // handle error
  auto donsus_syntax_error(DonsusParser::parse_result *node,
                           unsigned int column, unsigned int line,
                           const std::string &message) -> void;
  auto donsus_show_error_on_line(donsus_token_pos &ast_begin,
                                 donsus_token_pos &pos, donsus_token_pos &end)
      -> int;

  // creates a donsus_pos object from the current token so that it points to the
  // end
  auto donsus_end_pos(donsus_token &token) -> donsus_token_pos;

  auto donsus_make_pos_from_token(donsus_token &token) -> donsus_token_pos;

  // match for node to provide it
  auto extract(DonsusParser::parse_result node);
  donsus_token cur_token;
  donsus_lexer lexer;
  utility::handle<donsus_ast::tree> donsus_tree; // holds top level ast nodes
  utility::DonsusAllocator allocator;

private:
  DonsusParserError error;
  DonsusAstFile &file;
};
// debug
std::string de_get_name_from_token(donsus_token_kind kind);
DonsusParser Du_Parse(std::string result, DonsusAstFile &file);

#endif // PARSER_H