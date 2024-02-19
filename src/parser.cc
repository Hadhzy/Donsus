// pratt parser
#include "../Include/parser.h"
#include "../Include/donsus.h"
#include "./ast/node.h"
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
// DEBUG
void print_ast(utility::handle<donsus_ast::tree> tree) {
  std::cout << *tree->get_nodes()[0].get();
}

DonsusParser::DonsusParser(donsus_lexer &lexer) : error(false), lexer(lexer) {
  cur_token = donsus_lexer_next(*this);
  donsus_tree = new donsus_ast::tree();
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
    switch (cur_token.kind) {
    case DONSUS_NUMBER: {
      parse_result result = donsus_number_expr(0);
      donsus_tree->add_node(result);
    }
    case DONSUS_NAME: {
      /*      case DONSUS_BOOL:
            case DONSUS_VOID:
            case DONSUS_CHAR:
            case DONSUS_BASIC_INT:
            case DONSUS_I8:
            case DONSUS_I16:
            case DONSUS_I32:
            case DONSUS_I64:
            case DONSUS_U32:*/
      /*case DONSUS_U64:*/
      donsus_tree->add_node(donsus_variable_decl());
      break;
    }
    default: {
    }
    }
  }
#ifdef DEBUG
  std::cout << "AST: "
            << "\n";
  print_ast(donsus_tree);
#endif
  return donsus_tree;
}

auto DonsusParser::donsus_number_expr(unsigned int ptp) -> parse_result {
  // Gt the integer on the left
  parse_result left;
  parse_result right;
  parse_result global_node;

  left = donsus_number_primary(
      donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION,
      20); // return AST node with only value

  donsus_token previous_token = cur_token; // SAVE CUR_TOKEN

  donsus_parser_next();

  if (cur_token.kind == DONSUS_END) { // CHECK END
    return left;                      // return whole node
  }

  while (previous_token.precedence > ptp) {
    right = donsus_number_expr(previous_token.precedence); // recursive call

    global_node = create_number_expression(
        donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION, 10);

    auto &expression = global_node->get<donsus_ast::number_expr>();
    expression.value = previous_token;

    global_node->children.push_back(left);  // [0]
    global_node->children.push_back(right); // [1]

    if (cur_token.kind == DONSUS_END) {
      return global_node;
    }
  }
  return global_node;
}

auto DonsusParser::donsus_number_primary(donsus_ast::donsus_node_type type,
                                         uint64_t child_count) -> parse_result {
  const parse_result node = create_number_expression(
      donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION, 10);

  auto &expression = node->get<donsus_ast::number_expr>();
  expression.value = cur_token;

  donsus_parser_next();
  return node;
}

auto DonsusParser::donsus_expr() -> parse_result {
  // number expressions, string expressions etc.
  switch (cur_token.kind) {
  case DONSUS_NUMBER: {
    return donsus_number_expr(0);
  }
  }
}

auto DonsusParser::donsus_variable_definition(
    utility::handle<donsus_ast::node> &declaration) -> parse_result {
  // move to get the value
  donsus_parser_next();
  parse_result expression = donsus_expr();
  declaration->children.push_back(expression);
  return declaration;
}

auto DonsusParser::donsus_variable_decl() -> parse_result {
  // create an ast node with type DONSUS_DECLARATION
  // add this ast node to the top level AST
  // add this to the symbol table
  // figure out whether it has a definition
  /*  auto *n = new donsus_math_expr(cur_token, DONSUS_VAR_DECLARATION);
    return n;*/
  parse_result declaration = create_variable_declaration(
      donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION, 10);

  auto &expression = declaration->get<donsus_ast::variable_decl>();
  expression.identifier_name = cur_token.value;
  donsus_parser_next(); // expect next token to be ':'

  if (cur_token.kind == DONSUS_COLO) {
    donsus_parser_next(); // moves to the type
    expression.identifier_type = cur_token.kind;
    donsus_parser_next(); // if the next token is not '=' then its a
                          // declaration.
    if (cur_token.kind == DONSUS_EQUAL) {
      // def
      declaration->type =
          donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION; // overwrite
                                                                    // type
      return donsus_variable_definition(declaration);
    } else {
      // decl only
      if (cur_token.kind == DONSUS_SEMICOLON) {
        // end of declaration
        donsus_parser_next();
        return declaration;
      }
    }
  }
}

auto DonsusParser::create_number_expression(donsus_ast::donsus_node_type type,
                                            uint64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::number_expr>(type, child_count);
}

auto DonsusParser::create_variable_declaration(
    donsus_ast::donsus_node_type type, uint64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::variable_decl>(type, child_count);
}
