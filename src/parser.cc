// pratt parser
#include "../Include/parser.h"
#include "../Include/donsus.h"
#include <iostream>

/*
// HELPER
std::unique_ptr<donsus_ast> add_to_result(std::unique_ptr<donsus_ast> current,
                                          std::unique_ptr<donsus_ast> result) {}
*/

// DEBUG
static std::ostream &operator<<(std::ostream &o, donsus_token &token) {
  o << "Value: " << token.value << "\n";
  o << "Kind: " << de_get_name_from_token(token.kind) << "\n";
  o << "Length: " << token.length << "\n";
  o << "Line: " << token.line << "\n";
  return o;
}

DonsusParser::DonsusParser(donsus_lexer &lexer) : error(false), lexer(lexer) {
  cur_token = donsus_lexer_next(*this);
}

donsus_token DonsusParser::donsus_parser_next() {
  cur_token = donsus_lexer_next(*this);
  return cur_token;
}

// Todo: remove this from class
void DonsusParser::print_token() {
  while (cur_token.kind != DONSUS_END) {
    std::cout << cur_token << "\n"; // TODO: Implement this properly
    donsus_parser_next();
  }
}

donsus_token DonsusParser::donsus_peek() {
  donsus_lexer save = lexer;
  donsus_token result = donsus_lexer_next(*this);
  lexer = save;
  return result;
}

// TODO: proper debug for lexer and AST
auto DonsusParser::donsus_parse() -> end_result {
#ifdef DEBUG
  std::cout << "LEXER: "
            << "\n";
  DonsusParser save = *this;
  print_token();
  *this = save;
#endif

  while (cur_token.kind != DONSUS_END) {
    donsus_parser_next();
    switch (cur_token.kind) {
    case DONSUS_NUMBER:
      donsus_tree.add_node(donsus_number_expr(0));
    case DONSUS_BOOL:
    case DONSUS_VOID:
    case DONSUS_CHAR:
    case DONSUS_BASIC_INT:
    case DONSUS_I8:
    case DONSUS_I16:
    case DONSUS_I32:
    case DONSUS_I64:
    case DONSUS_U32:
    case DONSUS_U64: {
      // donsus_tree.add_node(donsus_variable_decl(cur_token.kind));
    }
    default: {
    }
    }
  }
  return donsus_tree;
  // #ifdef DEBUG
  // std::cout << "AST: " << "\n";
  // // print_ast(result);
  // #endif
  /*  return donsus_tree;*/
}

auto DonsusParser::donsus_number_expr(unsigned int ptp) -> parse_result {
  // Gt the integer on the left
  utility::handle<donsus_ast::node> left;
  utility::handle<donsus_ast::node> right;

  left = donsus_number_primary(
      donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
      20); // return AST node with only value

  donsus_token previous_token = cur_token; // SAVE CUR_TOKEN

  if (previous_token.kind == DONSUS_END) { // CHECK END
    return left;                           // return whole node
  }

  while (previous_token.precedence > ptp) {
    donsus_parser_next(); // get next token

    right = donsus_number_expr(previous_token.precedence); // recursive call

    left = create_node(donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
                       3, previous_token);

    left->children[0] = left;
    left->children[1] = right;

    if (cur_token.kind == DONSUS_END) {
      return left;
    }
  }
  return left;
}

auto DonsusParser::donsus_number_primary(donsus_ast::donsus_node_type type,
                                         uint64_t child_count) -> parse_result {
  const utility::handle<donsus_ast::node> node =
      create_node(type, child_count, cur_token);

  donsus_parser_next();
  return node;
}

auto DonsusParser::donsus_expr() -> parse_result {
  // number expressions, string expressions etc.
}

auto DonsusParser::donsus_variable_decl(donsus_token_kind type)
    -> parse_result {
  // create an ast node with type DONSUS_DECLARATION
  // add this ast node to the top level AST
  // add this to the symbol table
  // figure out whether it has a definition
  /*  auto *n = new donsus_math_expr(cur_token, DONSUS_VAR_DECLARATION);
    return n;*/
}
