//===----------------------------------------------------------------------===//
//
// Produces a tree data structure(AST) from the tokens.
// It uses pratt parsing and is responsible for making syntactic sense out of
// the language.
//===----------------------------------------------------------------------===//

#include "../Include/parser.h"
#include <iostream>
#include <utility>

std::vector<DonsusAstFile *> global_files;

class PrintAst {
public:
  void print_ast(utility::handle<donsus_ast::tree> tree) {
    int indent_level = 0;

    for (auto n : tree->get_nodes()) {
      print_ast_node(n, indent_level);
    }
  }

  void print_ast_node(utility::handle<donsus_ast::node> ast_node,
                      int indent_level) {
    if (!ast_node)
      return;

    using type = donsus_ast::donsus_node_type;
    switch (ast_node->type.type) {
    case type::DONSUS_VARIABLE_DECLARATION: {
      print_type(ast_node->type, indent_level);
      print_var_decl(ast_node->get<donsus_ast::variable_decl>(), indent_level);

      break;
    }

    case type::DONSUS_VARIABLE_DEFINITION: {
      print_type(ast_node->type, indent_level);
      indent_level++;
      print_var_decl(ast_node->get<donsus_ast::variable_decl>(),
                     indent_level); // reuse variable decl
      for (auto children_expr : ast_node->children) {
        print_with_newline("children: ", indent_level);
        print_ast_node(children_expr, indent_level + 2);
      }

      break;
    }

    case type::DONSUS_STRING_EXPRESSION: {
      print_type(ast_node->type, indent_level);
      print_string_expression(ast_node->get<donsus_ast::string_expr>(),
                              indent_level);
      break;
    }

    case type::DONSUS_BOOL_EXPRESSION: {
      print_type(ast_node->type, indent_level);
      print_bool_expression(ast_node->get<donsus_ast::bool_expr>(),
                            indent_level);
      break;
    }

    case type::DONSUS_UNARY_EXPRESSION: {
      print_type(ast_node->type, indent_level);
      print_with_newline("expression: ", indent_level);
      print_ast_node(ast_node->children[0], indent_level + 1);
      break;
    }

    case type::DONSUS_FUNCTION_ARG: {
      print_type(ast_node->type, indent_level);
      print_var_decl(ast_node->get<donsus_ast::variable_decl>(), indent_level);
      break;
    }

    case type::DONSUS_PRINT_EXPRESSION: {
      print_type(ast_node->type, indent_level);
      indent_level++;
      print_with_newline("expression: ", indent_level);
      print_ast_node(ast_node->children[0], indent_level + 1);
      break;
    }

    case type::DONSUS_ASSIGNMENT: {
      print_type(ast_node->type, indent_level);
      indent_level++;
      print_assignment(ast_node->get<donsus_ast::assignment>(), indent_level);

      for (auto children_expr : ast_node->children) {
        print_ast_node(children_expr, indent_level + 2);
      }
      break;
    }
    case type::DONSUS_NUMBER_EXPRESSION: {
      print_type(ast_node->type, indent_level);
      print_number_expr(ast_node->get<donsus_ast::number_expr>(), indent_level);

      if (ast_node->children.empty()) {
        print_with_newline("children: {}", indent_level);
      } else {
        print_with_newline("children: ", indent_level);
        for (auto children : ast_node->children) {
          print_ast_node(children, indent_level + 1);
          print_with_newline(" ", indent_level);
        }
      }
      break;
    }

    case type::DONSUS_FLOAT_EXPRESSION: {
      print_type(ast_node->type, indent_level);
      print_float_expression(ast_node->get<donsus_ast::float_expr>(),
                             indent_level);

      if (ast_node->children.empty()) {
        print_with_newline("children: {}", indent_level);
      } else {
        print_with_newline("children: ", indent_level);
        for (auto children : ast_node->children) {
          print_ast_node(children, indent_level + 1);
          print_with_newline(" ", indent_level);
        }
      }
      break;
    }

    case type::DONSUS_IDENTIFIER: {
      print_type(ast_node->type, indent_level);
      print_identifier(ast_node->get<donsus_ast::identifier>(), indent_level);
      break;
    }
    case type::DONSUS_FUNCTION_DECL: {
      print_type(ast_node->type, indent_level);
      print_function_decl(ast_node->get<donsus_ast::function_decl>(),
                          indent_level + 1);
      break;
    }

    case type::DONSUS_ARRAY_DECLARATION: {
      print_type(ast_node->type, indent_level);
      print_array_decl(ast_node->get<donsus_ast::array_decl>(), indent_level);
      break;
    }

    case type::DONSUS_ARRAY_DEFINITION: {
      print_type(ast_node->type, indent_level);
      print_array_def(ast_node->get<donsus_ast::array_def>(), indent_level);
      break;
    }

    case type::DONSUS_FUNCTION_DEF: {
      print_type(ast_node->type, indent_level);
      print_function_def(ast_node->get<donsus_ast::function_def>(),
                         indent_level + 1);
      break;
    }

    case type::DONSUS_IF_STATEMENT: {
      print_type(ast_node->type, indent_level);
      if (ast_node->children.empty()) {
        print_with_newline("condition: {}", indent_level);
      } else {
        print_with_newline("condition: ", indent_level);
        for (auto children : ast_node->children) {
          print_ast_node(children, indent_level + 1);
          print_with_newline(" ", indent_level);
        }
      }

      print_statement(ast_node->get<donsus_ast::if_statement>(), indent_level);

      break;
    }

    case type::DONSUS_FUNCTION_CALL: {
      print_type(ast_node->type, indent_level);
      print_function_call(ast_node->get<donsus_ast::function_call>(),
                          indent_level);
      break;
    }
    case type::DONSUS_ELSE_STATEMENT: {
      print_type(ast_node->type, indent_level);
      print_else_statement(ast_node->get<donsus_ast::else_statement>(),
                           indent_level);
      break;
    }

    case type::DONSUS_EXPRESSION: {
      print_type(ast_node->type, indent_level);
      print_expression(ast_node->get<donsus_ast::expression>(), indent_level);
      if (ast_node->children.empty()) {
        print_with_newline("children: {}", indent_level);
      } else {
        print_with_newline("children: ", indent_level);
        for (auto children : ast_node->children) {
          print_ast_node(children, indent_level + 1);
          print_with_newline(" ", indent_level);
        }
      }

      break;
    }

    case type::DONSUS_ARRAY_ACCESS: {
      print_type(ast_node->type, indent_level);
      print_with_newline(
          "identifier_name: " +
              ast_node->get<donsus_ast::array_access>().identifier_name,
          indent_level);
      print_ast_node(ast_node->get<donsus_ast::array_access>().index,
                     indent_level);
      break;
    }

    case type::DONSUS_RETURN_STATEMENT: {
      print_type(ast_node->type, indent_level);
      if (ast_node->children.empty()) {
        print_with_newline("children: {}", indent_level);
      } else {
        print_with_newline("children: ", indent_level);
        for (auto children : ast_node->children) {
          print_ast_node(children, indent_level + 1);
          print_with_newline(" ", indent_level);
        }
      }
      break;
    }

    default:
      break;
    }
  }

  void print_function_call(donsus_ast::function_call &f_call,
                           int indent_level) {
    print_with_newline("func_name: " + f_call.func_name, indent_level);
    print_with_newline("arguments: ", indent_level);
    for (auto p : f_call.arguments) {
      print_ast_node(p, indent_level + 1);
      print_with_newline(" ", indent_level);
    }
  }

  void print_print_expression(utility::handle<donsus_ast::node> &ast_node,
                              int indent_level) {
    print_ast_node(ast_node, indent_level + 1);
  }

  void print_number_expr(donsus_ast::number_expr &num_expr, int indent_level) {
    print_with_newline("kind: " + de_get_name_from_token(num_expr.value.kind),
                       indent_level);
    print_with_newline("value: " + num_expr.value.value, indent_level);
    print_with_newline("length: " + std::to_string(num_expr.value.length),
                       indent_level);
    print_with_newline("line: " + std::to_string(num_expr.value.line),
                       indent_level);
    print_with_newline("precedence: " +
                           std::to_string(num_expr.value.precedence),
                       indent_level);
  }

  void print_float_expression(donsus_ast::float_expr &float_expr,
                              int indent_level) {
    print_with_newline("kind: " + de_get_name_from_token(float_expr.value.kind),
                       indent_level);
    print_with_newline("value: " + float_expr.value.value, indent_level);
    print_with_newline("length: " + std::to_string(float_expr.value.length),
                       indent_level);
    print_with_newline("line: " + std::to_string(float_expr.value.line),
                       indent_level);
    print_with_newline("precedence: " +
                           std::to_string(float_expr.value.precedence),
                       indent_level);
  }

  void print_string_expression(donsus_ast::string_expr &string_expr,
                               int indent_level) {
    print_with_newline("kind: " +
                           de_get_name_from_token(string_expr.value.kind),
                       indent_level);
    print_with_newline("value: " + string_expr.value.value, indent_level);
    print_with_newline("length: " + std::to_string(string_expr.value.length),
                       indent_level);
    print_with_newline("line: " + std::to_string(string_expr.value.line),
                       indent_level);
    print_with_newline("precedence: " +
                           std::to_string(string_expr.value.precedence),
                       indent_level);
  }

  void print_bool_expression(donsus_ast::bool_expr &bool_expr,
                             int indent_level) {
    print_with_newline("kind: " + de_get_name_from_token(bool_expr.value.kind),
                       indent_level);
    print_with_newline("value: " + bool_expr.value.value, indent_level);
    print_with_newline("length: " + std::to_string(bool_expr.value.length),
                       indent_level);
    print_with_newline("line: " + std::to_string(bool_expr.value.line),
                       indent_level);
    print_with_newline("precedence: " +
                           std::to_string(bool_expr.value.precedence),
                       indent_level);
  }

  void print_identifier(donsus_ast::identifier &identifier, int indent_level) {
    print_with_newline("identifier_name: " + identifier.identifier_name,
                       indent_level);
  }

  void print_assignment(donsus_ast::assignment &assignment, int indent_level) {
    print_with_newline("lvalue: ", indent_level);
    print_ast_node(assignment.lvalue, indent_level + 1);
    print_with_newline("rvalue: ", indent_level);
    print_ast_node(assignment.rvalue, indent_level + 1);
    print_with_newline("identifier_op: " +
                           de_get_name_from_token(assignment.op.kind),
                       indent_level);
    print_with_newline("identifier_name: " + assignment.identifier_name,
                       indent_level);
  }

  void print_type(donsus_ast::donsus_node_type type, int indent_level) {
    print_with_newline("type: " + type.to_string(), indent_level);
  }

  void print_function_decl(donsus_ast::function_decl &f_decl,
                           int indent_level) {
    print_with_newline("return_types: ", indent_level);
    for (auto r : f_decl.return_type) {
      print_with_newline("return_type: " +
                             de_get_name_from_token(r.to_parse(r.type_un)),
                         indent_level + 1);
    }

    print_with_newline("parameters: ", indent_level);
    for (auto p : f_decl.parameters) {
      print_ast_node(p, indent_level + 1);
    }
    print_with_newline("func_name: " + f_decl.func_name, indent_level);
  }

  void print_function_def(donsus_ast::function_def &f_def, int indent_level) {
    print_with_newline("return_types: ", indent_level);
    for (auto r : f_def.return_type) {
      print_with_newline("return_type: " +
                             de_get_name_from_token(r.to_parse(r.type_un)),
                         indent_level + 1);
    }

    print_with_newline("parameters: ", indent_level);
    for (auto p : f_def.parameters) {
      print_ast_node(p, indent_level + 1);
    }
    print_with_newline("func_name: " + f_def.func_name, indent_level);
    print_with_newline("body: ", indent_level);
    for (auto node : f_def.body) {
      print_ast_node(node, indent_level + 1);
    }
  }

  void print_statement(donsus_ast::if_statement &statement, int indent_level) {
    print_with_newline("body: ", indent_level);
    for (auto node : statement.body) {
      print_ast_node(node, indent_level + 1);
    }
    if (statement.alternate.empty()) {
      print_with_newline("alternate: {}", indent_level);
    } else {
      for (auto node : statement.alternate) {
        print_with_newline("alternate: ", indent_level);
        print_ast_node(node, indent_level + 1);
      }
    }
  };

  void print_else_statement(donsus_ast::else_statement &statement,
                            int indent_level) {
    print_with_newline("body: ", indent_level);
    for (auto node : statement.body) {
      print_ast_node(node, indent_level + 1);
    }
  }

  void print_expression(donsus_ast::expression &expression, int indent_level) {
    print_with_newline("kind: " + de_get_name_from_token(expression.value.kind),
                       indent_level);
    print_with_newline("value: " + expression.value.value, indent_level);
    print_with_newline("length: " + std::to_string(expression.value.length),
                       indent_level);
    print_with_newline("line: " + std::to_string(expression.value.line),
                       indent_level);
    print_with_newline("precedence: " +
                           std::to_string(expression.value.precedence),
                       indent_level);
  }

  void print_array_decl(donsus_ast::array_decl &decl, int indent_level) {
    print_with_newline("identifier_type: " + de_get_name_from_token(decl.type),
                       indent_level);
    print_with_newline("identifier_name: " + decl.identifier_name,
                       indent_level);
    print_with_newline("size: " + std::to_string(decl.size), indent_level);
    print_with_newline("number_of_elements: " +
                           std::to_string(decl.number_of_elements),
                       indent_level);
  };

  void print_array_def(donsus_ast::array_def &def, int indent_level) {
    print_with_newline("identifier_type: " + de_get_name_from_token(def.type),
                       indent_level);
    print_with_newline("identifier_name: " + def.identifier_name, indent_level);
    print_with_newline("size: " + std::to_string(def.size), indent_level);
    print_with_newline("number_of_elements: " +
                           std::to_string(def.number_of_elements),
                       indent_level);
    print_with_newline("elements: ", indent_level);
    for (auto e : def.elements) {
      print_ast_node(e, indent_level + 1);
      print_with_newline(" ", indent_level);
    }
  };

  void print_with_newline(const std::string &s, int indent_level) {
    std::cout << generate_indentation(indent_level) << s << '\n';
  }

  void print_var_decl(donsus_ast::variable_decl &decl, int indent_level) {
    print_with_newline("identifier_type: " +
                           de_get_name_from_token(decl.identifier_type),
                       indent_level);
    print_with_newline("identifier_name: " + decl.identifier_name,
                       indent_level);
  };

  std::string generate_indentation(int level) {
    return std::string(2 * level, ' ');
  }
};

std::string generate_indentation(int level) {
  return std::string(2 * level, ' ');
};

static std::ostream &operator<<(std::ostream &o, donsus_token &token) {
  o << "Value: " << token.value << "\n";
  o << "Kind: " << de_get_name_from_token(token.kind) << "\n";
  o << "Length: " << token.length << "\n";
  o << "Line: " << token.line << "\n";
  return o;
}

DonsusParser::DonsusParser(donsus_lexer &lexer, DonsusAstFile &file)
    : lexer(lexer), allocator(1024), file(file) {
  cur_token = donsus_lexer_next(*this);
  // Todo: causes invalid read for some reasons in valgrind
  donsus_tree = allocator.r_alloc<donsus_ast::tree>();
  file.parser = this;
  global_files.push_back(&file);
}

donsus_token DonsusParser::donsus_parser_next() {
  cur_token = donsus_lexer_next(*this);
  return cur_token;
}

void DonsusParser::print_token() {
  while (cur_token.kind != DONSUS_END) {
    std::cout << cur_token << "\n"; //
    donsus_parser_next();
  }
}

donsus_token DonsusParser::donsus_peek(int loop) {
  donsus_lexer save = lexer;
  donsus_token result_main;
  for (int i = 0; i < loop; i++) {
    result_main = donsus_lexer_next(*this);
  }

  lexer = save;
  return result_main;
}

auto DonsusParser::donsus_parse() -> end_result {
#if DEBUG
  std::cout << "LEXER: "
            << "\n";
  DonsusParser save = *this;
  print_token();
  *this = save;
#endif
  while (cur_token.kind != DONSUS_END) {
    if (cur_token.kind == DONSUS_NAME) {
      if (donsus_peek().kind == DONSUS_LPAR) {
        parse_result result = donsus_function_decl();
        donsus_tree->add_node(result);
      } else if (donsus_peek().kind == DONSUS_COLO) {
        parse_result result = donsus_variable_decl();
        donsus_tree->add_node(result);
      } else if ((donsus_peek().kind == DONSUS_PLUS_EQUAL ||
                  donsus_peek().kind == DONSUS_MINUS_EQUAL ||
                  donsus_peek().kind == DONSUS_SLASH_EQUAL ||
                  donsus_peek().kind == DONSUS_STAR_EQUAL ||
                  donsus_peek().kind == DONSUS_EQUAL) ||
                 (donsus_peek().kind == DONSUS_LSQB)) {
        parse_result result = donsus_assignments();
        donsus_tree->add_node(result);
      } else if (donsus_peek().kind == DONSUS_COMM &&
                 donsus_peek(2).kind == DONSUS_NAME) {
        donsus_variable_multi_decl_def();
      } else {
        donsus_syntax_error(nullptr, cur_token.column, cur_token.line,
                            "Unexpected token: '" + cur_token.value);
      }
    }

    if (cur_token.kind == DONSUS_IF_KW) {
      parse_result result = donsus_if_statement();
      if (result->children.empty()) {
        donsus_syntax_error(&result, cur_token.column, cur_token.line,
                            "Condition wasn't provided for if statement \n");
      }
      donsus_tree->add_node(result);
    }

    if (cur_token.kind == DONSUS_PRINT_KW) {
      parse_result result = donsus_print();
      donsus_tree->add_node(result);
    } else if (cur_token.kind == DONSUS_FUNCTION_DEFINITION_KW) {
      parse_result result = donsus_function_definition();
      donsus_tree->add_node(result);
    }
    donsus_parser_next(); // move to the next token
                          // if (peek_function_definition()) {
                          // }
  }
#if DEBUG
  if (!file.error_count) {
    std::cout << "AST: "
              << "\n";
    PrintAst print_ast;

    print_ast.print_ast(donsus_tree);
  }
#endif
  return donsus_tree;
}

auto DonsusParser::donsus_variable_multi_decl_def() -> void {
  donsus_token start_offset;

  parse_result result;
  std::vector<std::string> identifier_names;
  donsus_token_kind type;
  bool is_def = false;
  while (cur_token.kind != DONSUS_SEMICOLON && cur_token.kind != DONSUS_END) {
    if (cur_token.kind == DONSUS_EQUAL) {
      is_def = true;
      donsus_parser_next(); // move to the expression
      result = donsus_expr(0);
    }
    if (cur_token.kind == DONSUS_COLO) {
      donsus_parser_next();
      if (!(DONSUS_TYPES_LEXER.find(cur_token.value) !=
            DONSUS_TYPES_LEXER.end()))
        donsus_syntax_error(nullptr, cur_token.column, cur_token.line,
                            "Type provided: '" + cur_token.value +
                                "' is not valid in the declaration of: '" +
                                identifier_names[0]);

      type = cur_token.kind;
    }
    if (cur_token.kind == DONSUS_COMM) {
      donsus_parser_next();
    }
    if (cur_token.kind == DONSUS_NAME) {
      start_offset = cur_token;
      identifier_names.push_back(cur_token.value);
    }
    if (cur_token.kind == DONSUS_SEMICOLON) {
      break;
    }

    donsus_parser_next();
  }

  for (auto &name : identifier_names) {
    parse_result declaration = create_variable_declaration(
        donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION, 10);
    declaration->start_offset_ast = start_offset;

    auto &expression = declaration->get<donsus_ast::variable_decl>();
    expression.identifier_name = cur_token.value;

    expression.identifier_name = name;
    expression.identifier_type = type;

    if (type == DONSUS_VOID) {
      donsus_syntax_error(&declaration, cur_token.column, cur_token.line,
                          "Void can't be used as a variable type");
    }

    if (is_def) {
      declaration->type =
          donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION;
      declaration->children.push_back(result);
    }

    donsus_tree->add_node(declaration);
  }
}

auto DonsusParser::donsus_number_primary(donsus_ast::donsus_node_type type,
                                         uint64_t child_count) -> parse_result {
  const parse_result node = create_number_expression(
      donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION, 10);

  auto &expression = node->get<donsus_ast::number_expr>();
  expression.value = cur_token;

  return node;
}

auto DonsusParser::donsus_float_number(donsus_ast::donsus_node_type type,
                                       uint64_t child_count) -> parse_result {
  const parse_result node = create_float_expression(
      donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION, 10);
  auto &expression = node->get<donsus_ast::float_expr>();
  expression.value = cur_token;

  return node;
}

auto DonsusParser::donsus_array_access() -> parse_result {
  donsus_token start_offset;

  parse_result node = create_array_access(
      donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS, 10);
  auto &expression = node->get<donsus_ast::array_access>();
  expression.identifier_name = cur_token.value;
  node->start_offset_ast = cur_token;

  donsus_parser_next(); // move to '['
  donsus_parser_next(); // move to the index
  // parse down index as expression
  auto index_expression = donsus_expr(0);
  expression.index = index_expression;

  donsus_parser_next(); // move to ']'
  return node;
}

// r-value expressions
/*
assignment_value:
    | assignment_start
    | arithmetic_expression

expressions:
    | assignment
    | arithmetic_expression
 * */
auto DonsusParser::match_expressions(unsigned int ptp) -> parse_result {
  // number expressions, string expressions etc.
  switch (cur_token.kind) {
  case DONSUS_LPAR:
  case DONSUS_NUMBER: {
    return donsus_number_primary(
        donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION, 10);
  }
  case DONSUS_FLOAT: {
    return donsus_float_number(
        donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION, 10);
  }

  case DONSUS_NAME: {
    if (donsus_peek().kind == DONSUS_LPAR) {
      return donsus_function_decl();
    } else if (donsus_peek().kind == DONSUS_LSQB) {
      return donsus_array_access();
    } else {
      return donsus_identifier();
    }
  }

  case DONSUS_STRING: {
    return string_expression();
  }

  case DONSUS_TRUE_KW:
  case DONSUS_FALSE_KW: {
    return bool_expression();
  }

  case DONSUS_MINUS: {
    return unary_expression();
  }

  default: {
    donsus_syntax_error(nullptr, cur_token.column, cur_token.line,
                        "Invalid expression provided at token: " +
                            cur_token.value);
    parse_result tmp;
    return tmp;
  }
  }
}

auto DonsusParser::make_new_expr_node(donsus_token prev_token,
                                      parse_result &left, parse_result &right)
    -> parse_result {
  parse_result new_expr_node;
  new_expr_node =
      create_expression(donsus_ast::donsus_node_type::DONSUS_EXPRESSION, 10);

  auto &expression = new_expr_node->get<donsus_ast::expression>();
  expression.value = std::move(prev_token);
  new_expr_node->children.push_back(left);
  new_expr_node->children.push_back(right);

  return new_expr_node;
}

auto DonsusParser::donsus_expr(unsigned int ptp) -> parse_result {
  // number expressions, string expressions etc.
  parse_result left;
  parse_result right;
  if (cur_token.kind == DONSUS_LPAR) {
    donsus_parser_next();
    left = donsus_expr(ptp);
  } else {
    left = match_expressions(ptp);
    if (!left) {
      parse_result tmp;
      return tmp;
    }
  }

  if (left) {
    if (left->type.type == donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS) {
      return left;
    }
  }
  donsus_parser_next();
  donsus_token previous_token = cur_token; // Save cur_token

  if (cur_token.kind == DONSUS_SEMICOLON || cur_token.kind == DONSUS_RPAR ||
      cur_token.kind == DONSUS_COMM)
    return left; // If there is only one expression

  while (previous_token.precedence > ptp) {
    donsus_parser_next();
    right = donsus_expr(previous_token.precedence);
    left = make_new_expr_node(previous_token, left, right);

    previous_token = cur_token;

    if (cur_token.kind == DONSUS_SEMICOLON || cur_token.kind == DONSUS_RPAR ||
        cur_token.kind == DONSUS_COMM) {
      // DONSUS_RPAR is for the condition of an if statement
      return left;
    }
  }

  if (left) {
    return left;
  } else {
    parse_result tmp;
    return tmp;
  }
}

/*
var_def:  DONSUS_NAME DONSUS_COLON donsus_type  DONSUS_EQUAL expression
 */
auto DonsusParser::donsus_variable_definition(
    utility::handle<donsus_ast::node> &declaration) -> parse_result {
  donsus_parser_next();
  parse_result expression = donsus_expr(0);
  declaration->children.push_back(expression);
  return declaration;
}

/*
 var_decl: DONSUS_NAME DONSUS_COLON donsus_type
*/
auto DonsusParser::donsus_variable_decl() -> parse_result {
  donsus_token start_offset;

  parse_result declaration = create_variable_declaration(
      donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION, 10);

  auto &expression = declaration->get<donsus_ast::variable_decl>();
  expression.identifier_name = cur_token.value;
  start_offset = cur_token;

  donsus_parser_except(DONSUS_COLO);

  donsus_token_kind type = cur_token.kind;

  if (cur_token.kind == DONSUS_COLO) {
    donsus_parser_next();
    // check for void explicitly

    donsus_token_kind type_m = cur_token.kind;
    if (type_m == DONSUS_VOID) {
      donsus_syntax_error(&declaration, cur_token.column, cur_token.line,
                          "Void can't be used as a variable type");
    }
    if (!(DONSUS_TYPES_LEXER.find(cur_token.value) != DONSUS_TYPES_LEXER.end()))
      donsus_syntax_error(&declaration, cur_token.column, cur_token.line,
                          "Type provided: '" + cur_token.value +
                              "' is not valid in the declaration of: '" +
                              expression.identifier_name);

    expression.identifier_type = cur_token.kind;

    if (donsus_peek().kind == DONSUS_EQUAL) {
      donsus_parser_next();
      declaration->type =
          donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION;

      if (donsus_peek().kind == DONSUS_SEMICOLON ||
          donsus_peek().kind == DONSUS_NEWLINE) {

        donsus_syntax_error(&declaration, cur_token.column, cur_token.line,
                            "Expected value after equal sign");
      }
      return donsus_variable_definition(declaration);
    } else if (donsus_peek().kind == DONSUS_LSQB) {

      // array
      donsus_parser_next(); // move to '['
      if (donsus_peek().kind == DONSUS_RSQB) {
        // dynamic array
        donsus_parser_next(); // move to ']'
        donsus_parser_next(); // move to '=' or ';'
        // check if its an array declaration or definition
        if (cur_token.kind == DONSUS_EQUAL) {
          return donsus_array_definition(declaration,
                                         donsus_ast::ArrayType::DYNAMIC, 0);
        } else if (cur_token.kind == DONSUS_SEMICOLON) {
          return donsus_array_declaration(declaration,
                                          donsus_ast::ArrayType::DYNAMIC, 0);
        }
      } else if (donsus_peek().kind == DONSUS_NUMBER) {
        // fixed size array

        donsus_parser_next(); // move to the number
        int size = std::stoi(cur_token.value);
        donsus_parser_next(); // move to ']'
        donsus_parser_next(); // move to '=' or ';' or .
        // check if its an array declaration or definition
        if (cur_token.kind == DONSUS_DOT) {
          donsus_parser_next(); // move to the next token
          if (cur_token.kind == DONSUS_EQUAL) {
            return donsus_array_definition(declaration,
                                           donsus_ast::ArrayType::STATIC, size);
          } else if (cur_token.kind == DONSUS_SEMICOLON) {
            return donsus_array_declaration(
                declaration, donsus_ast::ArrayType::STATIC, size);
          }
        }
        if (cur_token.kind == DONSUS_EQUAL) {
          return donsus_array_definition(declaration,
                                         donsus_ast::ArrayType::FIXED, size);
        } else if (cur_token.kind == DONSUS_SEMICOLON) {
          return donsus_array_declaration(declaration,
                                          donsus_ast::ArrayType::FIXED, size);
        }
      }
      // // array
      // donsus_parser_next(); // move to '['

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

auto DonsusParser::donsus_array_declaration(
    utility::handle<donsus_ast::node> &declaration,
    donsus_ast::ArrayType array_type, int size = 0) -> parse_result {
  // move to the next token
  parse_result array_declaration = create_array_declaration(
      donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION, 10);

  array_declaration->start_offset_ast = cur_token;

  auto &expression = array_declaration->get<donsus_ast::array_decl>();
  expression.identifier_name =
      declaration->get<donsus_ast::variable_decl>().identifier_name;
  expression.type =
      declaration->get<donsus_ast::variable_decl>().identifier_type;
  expression.array_type = array_type;

  return array_declaration;
}

auto DonsusParser::donsus_array_definition(
    utility::handle<donsus_ast::node> &declaration,
    donsus_ast::ArrayType array_type, int size = 0) -> parse_result {
  // move to the next token
  parse_result array_definition = create_array_definition(
      donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION, 10);

  array_definition->start_offset_ast = cur_token;

  auto &expression = array_definition->get<donsus_ast::array_def>();
  expression.identifier_name =
      declaration->get<donsus_ast::variable_decl>().identifier_name;
  expression.type =
      declaration->get<donsus_ast::variable_decl>().identifier_type;

  expression.array_type = array_type;

  expression.size = size;

  donsus_parser_except(DONSUS_LSQB); // expect next token to be '['
  donsus_parser_next();              // move to the next token
  while (cur_token.kind != DONSUS_RSQB) {
    parse_result element = donsus_expr(0);
    expression.elements.push_back(element);
    expression.number_of_elements++;
    if (cur_token.kind == DONSUS_COMM) {
      donsus_parser_next();
    }
  }

  return array_definition;
}

/*
 func_decl: DONSUS_NAME DONSUS_LPAR [donsus_function_proto] DONSUS_RPAR
 DONSUS_ARROW func_return_type
*/
auto DonsusParser::donsus_function_decl() -> parse_result {
  parse_result declaration = create_function_decl(
      donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL, 10);

  declaration->start_offset_ast = cur_token;

  auto &expression = declaration->get<donsus_ast::function_decl>();
  expression.func_name = cur_token.value;
  donsus_token name;
  // name'('
  name = cur_token;
  donsus_parser_except(DONSUS_LPAR);

  /*  if (cur_token.kind == DONSUS_NAME) {
      return donsus_function_call();
    }*/
  DONSUS_TYPE tmp{};
  if (donsus_peek().kind == DONSUS_NAME && donsus_peek(2).kind == DONSUS_COLO &&
      tmp.from_parse(donsus_peek(3).kind) != DONSUS_TYPE::TYPE_UNKNOWN) {
    // if we have parameters then the next token is DONSUS_NAME
    expression.parameters = donsus_function_signature(); // parse parameters
  }

  // check if it is
  if (tmp.from_parse(donsus_peek(3).kind) == DONSUS_TYPE::TYPE_UNKNOWN) {
    // without parameters
    /*
      donsus_peek(3) returns the place where the type is supposed to be if we
      can't match any type and the type is unknown(note: the type is not
      unknown, the type just simply doesn't exist, because that place is left
      empty) then we found a function_call().
    */
    return donsus_function_call(name);
  }
  // name params ')'
  donsus_parser_except(DONSUS_RPAR);

  // ARROW
  donsus_parser_except(DONSUS_ARROW);

  // ARROW--
  // parse type here
  donsus_parser_next();

  // if (donsus_peek().kind == DONSUS_LBRACE) {
  //   expression.return_type.push_back(make_type(cur_token.kind));
  // } else {
  //   // construct type
  //   while (donsus_peek().kind != DONSUS_LBRACE) {
  //     expression.return_type.push_back(make_type(cur_token.kind));
  //     donsus_parser_except(DONSUS_COMM);
  //     donsus_parser_next();
  //   }
  // }

  if (cur_token.kind == DONSUS_LBRACE || cur_token.kind == DONSUS_SEMICOLON) {

    donsus_syntax_error(&declaration, cur_token.column, cur_token.line,
                        "Return type wasn't provided for function: '" +
                            expression.func_name);
  }
  while (cur_token.kind != DONSUS_LBRACE &&
         cur_token.kind != DONSUS_SEMICOLON) {
    if (cur_token.kind == DONSUS_COMM)
      donsus_parser_next();
    if (DONSUS_TYPES_LEXER.find(cur_token.value) != DONSUS_TYPES_LEXER.end()) {
      expression.return_type.push_back(make_type(cur_token.kind));
    } else {
      donsus_syntax_error(&declaration, cur_token.column, cur_token.line,
                          "Return type received: '" + cur_token.value +
                              "' in invalid for function: '" +
                              expression.func_name);
    }
    donsus_parser_next();
  }

  return declaration;
}

auto DonsusParser::donsus_function_call(donsus_token &name) -> parse_result {
  parse_result function_call = create_function_call(
      donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL, 10);
  function_call->start_offset_ast = cur_token;

  auto &expression = function_call->get<donsus_ast::function_call>();
  expression.func_name = name.value;
  donsus_parser_next(); // move to arguments
  expression.arguments = donsus_function_args();
  donsus_parser_except_current(DONSUS_RPAR);

  return function_call;
}

/*
donsus_function_proto: var_decl* [,] [donsus_function_proto]
 */
auto DonsusParser::donsus_function_signature()
    -> std::vector<utility::handle<donsus_ast::node>> {
  // e.g (a:int, b:int)
  // As of now, parameters just variable decls(excluding semi-colon).
  std::vector<utility::handle<donsus_ast::node>> a;

  while (donsus_peek().kind != DONSUS_RPAR) {
    try {
      donsus_parser_except(DONSUS_NAME);
      parse_result v_d = donsus_variable_decl(); // catch its value
      v_d->type = donsus_ast::donsus_node_type::DONSUS_FUNCTION_ARG;

      a.push_back(v_d);
    } catch (DonsusException &e) {
      std::cerr << e.what() << std::endl;
    }

    if (donsus_peek().kind == DONSUS_COMM) {
      donsus_parser_next();
      continue;
    } else {
      break; // finished
    }
  }
  return a;
}

auto DonsusParser::donsus_function_args()
    -> std::vector<utility::handle<donsus_ast::node>> {
  std::vector<utility::handle<donsus_ast::node>> a;
  while (cur_token.kind != DONSUS_RPAR) {
    parse_result argument = donsus_expr(0);
    a.push_back(argument);
    if (cur_token.kind == DONSUS_COMM) {
      donsus_parser_next();
    }
  }
  return a;
}

/*
func_def: DONSUS_NAME DONSUS_LPAR [donsus_function_proto] DONSUS_RPAR
DONSUS_ARROW func_return_type DONSUS_LBRACE expression_statements
DONSUS_RBRACE
 * */
auto DonsusParser::donsus_function_definition() -> parse_result {
  // parse smaller parts such as statements | assignments |
  donsus_parser_except(DONSUS_NAME); // after "def"we have a DONSUS_NAME
  parse_result definition = create_function_definition(
      donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF, 10);

  definition->start_offset_ast = cur_token;
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

  donsus_parser_except_current(DONSUS_LBRACE); // expect cur_token to be "{"
  definition_expression.body = donsus_statements();
  return definition;
}

/*
statements: |general_statement+
            |expression_statements
*/
auto DonsusParser::donsus_statements() -> std::vector<parse_result> {
  std::vector<parse_result> body;
  while (cur_token.kind != DONSUS_RBRACE) {
    /* general_statement:
        | func_def */
    if (cur_token.kind == DONSUS_FUNCTION_DEFINITION_KW) {
      parse_result result = donsus_function_definition();
      body.push_back(result);
    }

    if (cur_token.kind == DONSUS_PRINT_KW) {
      parse_result result = donsus_print();
      body.push_back(result);
    }

    /*
      | func_decl
     */
    if (cur_token.kind == DONSUS_NAME) {
      if (donsus_peek().kind == DONSUS_LPAR) {
        parse_result result = donsus_function_decl();
        body.push_back(result);
      } else if (donsus_peek().kind == DONSUS_COLO) {
        /*
          | var_decl
         */
        parse_result result = donsus_variable_decl();
        body.push_back(result);
      }
    }

    if (cur_token.kind == DONSUS_IF_KW) {
      parse_result result = donsus_if_statement();
      if (result->children.empty()) {
        donsus_syntax_error(&result, cur_token.column, cur_token.line,
                            "Condition wasn't provided for if statement \n");
      }
      body.push_back(result);
    }
    donsus_parser_next();
    // assignments
    /*
     assignment_start:
    | DONSUS_NAME
    | DONSUS_NUMBER
    | func_call
    */
    if ((cur_token.kind == DONSUS_NAME || cur_token.kind == DONSUS_NUMBER)) {
      /*
        assignment_op:
        | DONSUS_PLUS_EQUAL
        | DONSUS_MINUS_EQUAL
        | DONSUS_STAR_EQUAL
        | DONSUS_SLASH_EQUAL
        */
      auto next_token = donsus_peek().kind;
      if (next_token == DONSUS_PLUS_EQUAL || next_token == DONSUS_MINUS_EQUAL ||
          next_token == DONSUS_STAR_EQUAL || next_token == DONSUS_SLASH_EQUAL ||
          next_token == DONSUS_EQUAL) {
        parse_result result = donsus_assignments();
        body.push_back(result);
      }
    }

    if (cur_token.kind == DONSUS_RETURN_KW) {
      parse_result result = donsus_return_statement();
      body.push_back(result);
    }
  }

  return body;
}

auto DonsusParser::donsus_if_statement() -> parse_result {

  parse_result statement = create_if_statement(
      donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT, 10);

  statement->start_offset_ast = cur_token;

  auto &statement_expression = statement->get<donsus_ast::if_statement>();

  donsus_parser_except(DONSUS_LPAR); // after "if" we have "("
  donsus_parser_next(); // go to next token which will be an expression

  while (cur_token.kind != DONSUS_RPAR) {
    parse_result condition_expression = donsus_expr(0);
    statement->children.push_back(condition_expression);
  }

  donsus_parser_except(DONSUS_LBRACE); // after ")" we have "{"
  statement_expression.body = donsus_statements();
  donsus_parser_next(); // get next token potential elif, else
  if (cur_token.kind == DONSUS_ELIF_KW) {
    parse_result result = donsus_if_statement();
    statement_expression.alternate.push_back(result);
  }

  if (cur_token.kind == DONSUS_ELSE_KW) {
    parse_result result = donsus_else_statement();
    statement_expression.alternate.push_back(result);
  }

  return statement;
}

auto DonsusParser::donsus_else_statement() -> parse_result {
  parse_result else_statement = create_else_statement(
      donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT, 10);

  else_statement->start_offset_ast = cur_token;

  auto &expression = else_statement->get<donsus_ast::else_statement>();

  donsus_parser_except(DONSUS_LBRACE); // expect cur_token to be "{"
  expression.body = donsus_statements();

  return else_statement;
}

auto DonsusParser::donsus_identifier() -> parse_result {
  parse_result result =
      create_identifier(donsus_ast::donsus_node_type::DONSUS_IDENTIFIER, 10);
  result->start_offset_ast = cur_token;
  auto &expression = result->get<donsus_ast::identifier>();
  expression.identifier_name = cur_token.value;
  return result;
}

auto DonsusParser::string_expression() -> parse_result {
  parse_result result = create_string_expression(
      donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION, 10);
  result->start_offset_ast = cur_token;

  auto &expression = result->get<donsus_ast::string_expr>();
  expression.value = cur_token;
  return result;
}

auto DonsusParser::bool_expression() -> parse_result {
  parse_result result = create_bool_expression(
      donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION, 10);
  result->start_offset_ast = cur_token;

  auto &expression = result->get<donsus_ast::bool_expr>();
  expression.value = cur_token;
  return result;
}

auto DonsusParser::unary_expression() -> parse_result {
  donsus_parser_next();
  parse_result result = create_expression(
      donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION, 10);
  result->start_offset_ast = cur_token;

  auto &expression = result->get<donsus_ast::unary_expr>();
  expression.op = cur_token;
  parse_result unary = donsus_expr(0);
  result->children.push_back(unary);
  return result;
}

auto DonsusParser::donsus_print() -> parse_result {
  parse_result print = create_donsus_print(
      donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION, 10);
  print->start_offset_ast = cur_token;

  donsus_parser_except(DONSUS_LPAR);

  donsus_parser_next(); // move to expression
  while (cur_token.kind != DONSUS_RPAR) {
    parse_result expression = donsus_expr(0);
    print->children.push_back(expression);
    if (cur_token.kind == DONSUS_COMM) {
      donsus_parser_next();
      continue;
    }
  }

  donsus_parser_except_current(DONSUS_RPAR);
  donsus_parser_except(DONSUS_SEMICOLON);

  return print;
}

auto DonsusParser::donsus_return_statement() -> parse_result {
  parse_result return_statement = create_return_statement(
      donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT, 10);
  return_statement->start_offset_ast = cur_token;

  donsus_parser_next();
  parse_result return_expression = donsus_expr(0);
  return_statement->children.push_back(return_expression);
  return return_statement;
}

/*
assignment: | assignment_start assignment_op assignment_value
assignment_op:
               | DONSUS_PLUS_EQUAL
               | DONSUS_MINUS_EQUAL
               | DONSUS_STAR_EQUAL
               | DONSUS_SLASH_EQUAL

assignment_value:
    | assignment_start
    | arithmetic_expression
 */
auto DonsusParser::donsus_assignments() -> parse_result {
  parse_result assignment =
      create_assignments(donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT, 10);

  assignment->start_offset_ast = cur_token;

  auto &expression = assignment->get<donsus_ast::assignment>();
  // parse lvalue as an expression
  if (cur_token.kind == DONSUS_NAME) {
    expression.identifier_name = cur_token.value;
  }
  parse_result lvalue = donsus_expr(0);
  expression.lvalue = lvalue;

  // donsus_parser_next(); // one of the possible assignment operators
  /*
   | DONSUS_PLUS_EQUAL
   | DONSUS_MINUS_EQUAL
   | DONSUS_STAR_EQUAL
   | DONSUS_SLASH_EQUAL
   | DONSUS_EQUAL
                       * */

  expression.op = cur_token;
  donsus_parser_next();
  while (cur_token.kind != DONSUS_SEMICOLON) {
    parse_result expression_child = donsus_expr(0);
    expression.rvalue = expression_child;
    if (cur_token.kind == DONSUS_SEMICOLON) {
      break;
    }
    donsus_parser_next();
    donsus_parser_next(); // operator here
  }

  /*
   assignment_value:
  | assignment_start
  | arithmetic_expression
   * */

  return assignment;
}

auto DonsusParser::create_number_expression(donsus_ast::donsus_node_type type,
                                            uint64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::number_expr>(type, child_count);
}

auto DonsusParser::create_string_expression(donsus_ast::donsus_node_type type,
                                            uint64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::string_expr>(type, child_count);
}

auto DonsusParser::create_bool_expression(donsus_ast::donsus_node_type type,
                                          uint64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::bool_expr>(type, child_count);
}

auto DonsusParser::create_unary_expression(donsus_ast::donsus_node_type type,
                                           uint64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::unary_expr>(type, child_count);
}

auto DonsusParser::create_float_expression(donsus_ast::donsus_node_type type,
                                           u_int64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::float_expr>(type, child_count);
}

auto DonsusParser::create_donsus_print(donsus_ast::donsus_node_type type,
                                       uint64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::print_expr>(type, child_count);
}

auto DonsusParser::create_variable_declaration(
    donsus_ast::donsus_node_type type, uint64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::variable_decl>(type, child_count);
}

auto DonsusParser::create_array_declaration(donsus_ast::donsus_node_type type,
                                            u_int64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::array_decl>(type, child_count);
}

auto DonsusParser::create_array_definition(donsus_ast::donsus_node_type type,
                                           u_int64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::array_def>(type, child_count);
}

auto DonsusParser::create_function_decl(donsus_ast::donsus_node_type type,
                                        u_int64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::function_decl>(type, child_count);
}

auto DonsusParser::create_if_statement(donsus_ast::donsus_node_type type,
                                       u_int64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::if_statement>(type, child_count);
}

auto DonsusParser::create_else_statement(donsus_ast::donsus_node_type type,
                                         u_int64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::else_statement>(type,
                                                              child_count);
}

auto DonsusParser::create_function_definition(donsus_ast::donsus_node_type type,
                                              u_int64_t child_count)
    -> parse_result {
  return donsus_tree->create_node<donsus_ast::function_def>(type, child_count);
}

auto DonsusParser::create_assignments(donsus_ast::donsus_node_type type,
                                      u_int64_t child_count) -> parse_result {

  return donsus_tree->create_node<donsus_ast::assignment>(type, child_count);
}

auto DonsusParser::create_return_statement(donsus_ast::donsus_node_type type,
                                           u_int64_t child_count)
    -> parse_result {

  return donsus_tree->create_node<donsus_ast::return_kw>(type, child_count);
}

auto DonsusParser::create_array_access(donsus_ast::donsus_node_type type,
                                       u_int64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::array_access>(type, child_count);
}

auto DonsusParser::create_identifier(donsus_ast::donsus_node_type type,
                                     u_int64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::identifier>(type, child_count);
}

auto DonsusParser::create_expression(donsus_ast::donsus_node_type type,
                                     u_int64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::expression>(type, child_count);
}

auto DonsusParser::create_function_call(donsus_ast::donsus_node_type type,
                                        u_int64_t child_count) -> parse_result {
  return donsus_tree->create_node<donsus_ast::function_call>(type, child_count);
}

auto DonsusParser::donsus_syntax_error(DonsusParser::parse_result *node,
                                       unsigned int column, unsigned int line,
                                       const std::string &message) -> void {
  donsus_token_pos pos{};
  pos.offset = 0;
  pos.line = line;
  pos.column = column;
  pos.file_id = file.id;
  pos.abs_offset = cur_token.offset;

  donsus_token_pos end = donsus_end_pos(cur_token);

  error.syntax_error_normal(column, line, message, file.absolute_path);
  if (!node) {
    file.error_count += 1;
    error.error_out_coloured("", rang::fg::reset);
    return;
  }
  donsus_token ast_first_token = node->get()->start_offset_ast;
  donsus_token_pos ast_first_token_pos =
      donsus_make_pos_from_token(ast_first_token);
  donsus_show_error_on_line(ast_first_token_pos, pos, end);
  // reset
  error.error_out_coloured("", rang::fg::reset);
  file.error_count += 1;
}
auto DonsusParser::donsus_show_error_on_line(donsus_token_pos &ast_begin,
                                             donsus_token_pos &pos,
                                             donsus_token_pos &end) -> int {
  std::string value_c = file.parser->lexer.string;

  std::string whole_ast_range =
      value_c.substr(ast_begin.abs_offset, cur_token.offset);

  std::string token_range = value_c.substr(pos.abs_offset, end.abs_offset);

  error.error_out_coloured(whole_ast_range, rang::fg::reset);
  error.error_out_coloured(token_range, rang::fg::reset);

  for (int i = 0; i < cur_token.column - cur_token.length; i++) {
    error.error_out_empty();
  }
  error.error_out_coloured("^", rang::fg::green);

  for (int i = 0; i < cur_token.length; i++) {
    error.error_out_coloured("~", rang::fg::green);
  }

  error.error_out_coloured("^", rang::fg::green);
  error.error_out_coloured("\n");
  return 0;
  // printing out ~~~~~~~
}
// Creates a donsus pos which points to the end of the token
auto DonsusParser::donsus_end_pos(donsus_token &token) -> donsus_token_pos {

  donsus_token_pos c_pos{};
  c_pos.offset += token.length;
  for (unsigned int i = 0; i < token.length; i++) {
    char c = token.value[i];
    if (c == '\n') {
      c_pos.line += 1;
      c_pos.column = 1;
    } else {
      c_pos.column += 1;
    }
  }
  c_pos.abs_offset = cur_token.offset + c_pos.offset;
  c_pos.column = token.column;
  c_pos.line = token.line;
  c_pos.file_id = file.id;
  return c_pos;
}
// avo copying
auto DonsusParser::donsus_make_pos_from_token(donsus_token &token)
    -> donsus_token_pos {
  donsus_token_pos pos{};
  pos.file_id = file.id;
  pos.line = token.line;
  pos.column = token.column;

  pos.abs_offset = token.offset;
  return pos;
}
// Throws exception
void DonsusParser::donsus_parser_except(donsus_token_kind type) {
  donsus_token a = donsus_parser_next();
  if (a.kind != type) {
    // exception here
    donsus_syntax_error(
        nullptr, cur_token.column, cur_token.line,
        "Expected token:" + de_get_name_from_token(type) +
            " got instead: " + de_get_name_from_token(cur_token.kind) + "\n");
  }
}

void DonsusParser::donsus_parser_except_current(donsus_token_kind type) {
  if (cur_token.kind != type) {
    // exception here
    donsus_syntax_error(
        nullptr, cur_token.column, cur_token.line,
        "Expected token:" + de_get_name_from_token(type) +
            " got instead: " + de_get_name_from_token(cur_token.kind) + "\n");
  }
}