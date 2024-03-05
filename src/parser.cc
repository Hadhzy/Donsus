// pratt parser
#include "../Include/parser.h"
#include <iostream>

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
  for (auto n : tree->get_nodes()) {

    std::cout << n->type.to_string() << "\n";
  }
}
DonsusParser::DonsusParser(donsus_lexer &lexer) : lexer(lexer) {
  cur_token = donsus_lexer_next(*this);
  donsus_tree = new donsus_ast::tree();
}

donsus_token DonsusParser::donsus_parser_next() {
  cur_token = donsus_lexer_next(*this);
  return cur_token;
}

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

auto DonsusParser::donsus_parse() -> end_result {
#ifdef DEBUG
  std::cout << "LEXER: "
            << "\n";
  DonsusParser save = *this;
  print_token();
  *this = save;
#endif
  while (cur_token.kind != DONSUS_END) {
    // switch (cur_token.kind) {
    // case DONSUS_NUMBER: {
    //   parse_result result = donsus_number_expr(0);
    //   donsus_tree->add_node(result);
    // }
    // case DONSUS_NAME: {
    //   /*      case DONSUS_BOOL:
    //         case DONSUS_VOID:
    //         case DONSUS_CHAR:
    //         case DONSUS_BASIC_INT:
    //         case DONSUS_I8:
    //         case DONSUS_I16:
    //         case DONSUS_I32:
    //         case DONSUS_I64:
    //         case DONSUS_U32:*/
    //   /*case DONSUS_U64:*/
    //   donsus_tree->add_node(donsus_variable_decl());
    //   break;
    // }
    // default: {
    // }
    // }
    // Doesn't support globals
    if (cur_token.kind == DONSUS_NAME) {
      if (donsus_peek().kind == DONSUS_LPAR) {
        parse_result result = donsus_function_decl();
        donsus_tree->add_node(result);
      } else if (donsus_peek().kind == DONSUS_COLO) {
        parse_result result = donsus_variable_decl();
        donsus_tree->add_node(result);
      }
    }

    if (cur_token.kind == DONSUS_FUNCTION_DEFINITION_KW) {
      parse_result result = donsus_function_definition();
      donsus_tree->add_node(result);
    }
    donsus_parser_next(); // move to the next token
    // if (peek_function_definition()) {
    // }
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

  donsus_parser_next();

  donsus_token previous_token = cur_token; // SAVE CUR_TOKEN

  if (cur_token.kind == DONSUS_SEMICOLON) { // CHECK END
    return left;                            // return whole node
  }

  donsus_parser_next();

  while (previous_token.precedence > ptp) {
    right = donsus_number_expr(previous_token.precedence); // recursive call

    global_node = create_number_expression(
        donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION, 10);

    auto &expression = global_node->get<donsus_ast::number_expr>();
    expression.value = previous_token;

    global_node->children.push_back(left);  // [0]
    global_node->children.push_back(right); // [1]

    if (cur_token.kind == DONSUS_SEMICOLON) {
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
  donsus_parser_except(DONSUS_NUMBER);
  parse_result expression = donsus_expr();
  declaration->children.push_back(expression);
  return declaration;
}

auto DonsusParser::donsus_variable_decl() -> parse_result {
  // Starts with DONSUS_NAME
  // ENDS with  parsing SEMI_COLON
  // CALL definition if needed
  // RETURNS its structure without parsing semi_colon if's called from
  // variable_decl
  parse_result declaration = create_variable_declaration(
      donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION, 10);

  auto &expression = declaration->get<donsus_ast::variable_decl>();
  expression.identifier_name = cur_token.value;

  // add stuff to symbol_table
  donsus_parser_except(DONSUS_COLO); // expect next token to be ':'

  if (cur_token.kind == DONSUS_COLO) {
    donsus_parser_next(); // moves to the type
    expression.identifier_type = cur_token.kind;

    /*    donsus_parser_next(); // if the next token is not '=' then its a
                              // declaration.*/
    if (donsus_peek().kind == DONSUS_EQUAL) {
      // def
      donsus_parser_next();
      declaration->type = donsus_ast::donsus_node_type::
          DONSUS_VARIABLE_DEFINITION; // overwrite//
                                      // type
      return donsus_variable_definition(declaration);
    } else {

      // decl only
      if (donsus_peek().kind == DONSUS_SEMICOLON) {
        donsus_parser_except(DONSUS_SEMICOLON);
        // end of declaration
        return declaration;
      } else {
        return declaration;
      }
    }
  }
}

auto DonsusParser::donsus_function_decl() -> parse_result {
  parse_result declaration = create_function_decl(
      donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL, 10);

  auto &expression = declaration->get<donsus_ast::function_decl>();
  expression.func_name = cur_token.value;

  // name'('
  donsus_parser_except(DONSUS_LPAR);
  if (donsus_peek().kind == DONSUS_NAME) {
    // if we have parameters then the next token is DONSUS_NAME
    expression.parameters = donsus_function_signature(); // parse parameters
  }
  // name params ')'
  donsus_parser_except(DONSUS_RPAR);

  // ARROW
  donsus_parser_except(DONSUS_ARROW);

  // ARROW--
  // parse type here
  donsus_parser_next();

  /*  if (donsus_peek().kind == DONSUS_LBRACE){
      expression.return_type.push_back(make_type(cur_token.kind));
    } else {
      // construct type
      while (donsus_peek().kind != DONSUS_LBRACE){
        expression.return_type.push_back(make_type(cur_token.kind));
        donsus_parser_except(DONSUS_COMM);
        donsus_parser_next();
      }

    }*/
  expression.return_type.push_back(make_type(cur_token.kind));

  while (donsus_peek().kind != DONSUS_LBRACE &&
         donsus_peek().kind != DONSUS_SEMICOLON) {
    donsus_parser_except(DONSUS_COMM);
    donsus_parser_next();
    expression.return_type.push_back(make_type(cur_token.kind));
  }

  return declaration;
}
auto DonsusParser::donsus_function_signature() -> std::vector<NAME_DATA_PAIR> {
  // e.g (a:int, b:int)
  // As of now, parameters just variable decls(excluding semi-colon).
  std::vector<NAME_DATA_PAIR> a;

  while (donsus_peek().kind != DONSUS_RPAR) {
    NAME_DATA_PAIR pair;
    donsus_parser_except(DONSUS_NAME);
    parse_result v_d = donsus_variable_decl(); // catch its value
    pair.identifier =
        v_d.get()->get<donsus_ast::variable_decl>().identifier_name;
    pair.type =
        make_type(v_d.get()->get<donsus_ast::variable_decl>().identifier_type);
    a.push_back(pair);

    if (donsus_peek().kind == DONSUS_COMM) {
      donsus_parser_next();
      continue;
    } else {
      break; // finished
    }
  }
  return a;
}

auto DonsusParser::donsus_function_definition() -> parse_result {
  // parse smaller parts such as statements | assignments |
  donsus_parser_except(DONSUS_NAME); // after "def"we have a DONSUS_NAME
  parse_result definition = create_function_definition(
      donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF, 10);

  // construct sym_table

  auto &definition_expression = definition->get<donsus_ast::function_def>();

  // since up to the return type function def is the same as function decl
  // excluding semicolon

  // we can re-use function_decl to parse up to the return type
  parse_result declaration =
      donsus_function_decl(); // get the declaration for definition
  auto &declaration_expression = declaration->get<donsus_ast::function_decl>();

  // They share all the properties excluding the "body"
  definition_expression.func_name = declaration_expression.func_name;
  definition_expression.parameters = declaration_expression.parameters;
  definition_expression.return_type = declaration_expression.return_type;

  donsus_parser_except(DONSUS_LBRACE); // expect cur_token to be "{"
  definition_expression.body = donsus_statements();
  return definition;
}

auto DonsusParser::donsus_statements() -> std::vector<parse_result> {
  std::vector<parse_result> body;
  while (cur_token.kind != DONSUS_RBRACE) {
    // parse statements
    if (cur_token.kind == DONSUS_FUNCTION_DEFINITION_KW) {
      parse_result result = donsus_function_definition();
      body.push_back(result);
    }

    if (cur_token.kind == DONSUS_NAME) {
      if (donsus_peek().kind == DONSUS_LPAR) {
        parse_result result = donsus_function_decl();
        body.push_back(result);
      } else if (donsus_peek().kind == DONSUS_COLO) {
        parse_result result = donsus_variable_decl();
        body.push_back(result);
      }
    }
    donsus_parser_next();
  }

  return body;
}

// Todo: Finish this:
/*auto DonsusParser::peek_is_function_definition() -> bool {
  if (peek_for_token().kind != DONSUS_NAME) {
    return false;
  }

  const bool res = peek_for_token().kind == DONSUS_LPAR;
}*/
// Todo: Make it more accurate

auto DonsusParser::peek_is_function_declaration() -> bool {
  if (cur_token.kind != DONSUS_NAME) {
    return false;
  }
  if (donsus_peek().kind == DONSUS_LPAR && cur_token.kind == DONSUS_NAME) {
    return true;
  }
  return false;
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

auto DonsusParser::create_function_decl(donsus_ast::donsus_node_type type,
                                        u_int64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::function_decl>(type, child_count);
}

auto DonsusParser::create_function_definition(donsus_ast::donsus_node_type type,
                                              u_int64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::function_def>(type, child_count);
}

// Throws exception
void DonsusParser::donsus_parser_except(donsus_token_kind type) {
  donsus_token a = donsus_parser_next();
  if (a.kind != type) {
    // exception here
    throw DonsusException(
        "Expected token:" + de_get_name_from_token(type) +
        " got instead: " + de_get_name_from_token(cur_token.kind) + "\n" +
        "at line: " + std::to_string(lexer.cur_line));
  }
}
