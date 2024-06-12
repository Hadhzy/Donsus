//===----------------------------------------------------------------------===//
//
// Performs semantic analysis and typechecking recursively.
// It assigns DONSUS_TYPE(s) to nodes then performs typechecking by matching
// different properties with each other. It might also as well check for
// whether a concept contextually makes sense - although this is highly unlikely
// as the parser catches most of the mistakes and processes everything.
//===----------------------------------------------------------------------===//
#include "sema.h"

auto DonsusSema::process_donsus_expression(
    utility::handle<donsus_ast::node> node,
    utility::handle<DonsusSymTable> table,
    utility::handle<DonsusSymTable> global_table) -> void {
  for (auto n : node->children) {
    if (n->type.type == donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
      process_donsus_expression(n, table, global_table);
    } else {
      assign_type_to_node(n, table, global_table);
    }
  }
  // go until find a proper type
}

/*
 Returns the type of the DONSUS_EXPRESSION based on its children
 * */
auto DonsusSema::decide_type_for_expression(
    utility::handle<donsus_ast::node> node,
    utility::handle<DonsusSymTable> table) -> DONSUS_TYPE {
  for (auto n : node->children) {
    if (n->type.type != donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
      return n->real_type;
    }
  }
}

auto DonsusSema::assign_type_to_node(
    utility::handle<donsus_ast::node> node,
    utility::handle<DonsusSymTable> table,
    utility::handle<DonsusSymTable> global_table) -> void {

  switch (node->type.type) {

  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
    // we can't figure out the type here.
    // should be compatible with other types
    node->real_type.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS: {
    // make sure the array is defined
    // make sure the index is correct
    // assign the type based on the array

    std::string array_name =
        node->get<donsus_ast::array_access>().identifier_name;
    bool is_exist = donsus_sema_is_exist(array_name, table);
    bool is_exist_global = donsus_sema_is_exist(array_name, global_table);
    if (!is_exist && !is_exist_global)
      throw DonsusUndefinedException(array_name + " is not defined");

    DONSUS_TYPE array_type = table->get(array_name).array.type;
    node->real_type = array_type;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION: {
    for (auto &n : node->get<donsus_ast::array_def>().elements) {
      assign_type_to_node(n, table, global_table);
    }
    switch (node->get<donsus_ast::array_def>().array_type) {
    case donsus_ast::DYNAMIC: {
      node->real_type.type_un = DONSUS_TYPE::TYPE_DYNAMIC_ARRAY;
      break;
    }
    case donsus_ast::FIXED: {
      node->real_type.type_un = DONSUS_TYPE::TYPE_FIXED_ARRAY;
      break;
    }
    case donsus_ast::STATIC: {
      node->real_type.type_un = DONSUS_TYPE::TYPE_STATIC_ARRAY;
      break;
    }
    }
  }
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION: {
    switch (node->get<donsus_ast::array_decl>().array_type) {
    case donsus_ast::DYNAMIC: {
      node->real_type.type_un = DONSUS_TYPE::TYPE_DYNAMIC_ARRAY;
      break;
    }
    case donsus_ast::FIXED: {
      node->real_type.type_un = DONSUS_TYPE::TYPE_FIXED_ARRAY;
      break;
    }
    case donsus_ast::STATIC: {
      node->real_type.type_un = DONSUS_TYPE::TYPE_STATIC_ARRAY;
      break;
    }
    }
  }

  case donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION: {
    node->real_type.type_un = DONSUS_TYPE::TYPE_F32;
    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {

    node->real_type.type_un =
        make_type(node->get<donsus_ast::variable_decl>().identifier_type)
            .type_un;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_ARG: {
    node->real_type.type_un =
        make_type(node->get<donsus_ast::variable_decl>().identifier_type)
            .type_un;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
    process_donsus_expression(node, table, global_table);
    donsus_typecheck_support_between_types(node);
    // after all the children of donsus expression got a type
    // we can just pick the closest one and set it up as the expression
    // type
    node->real_type.type_un = decide_type_for_expression(node, table).type_un;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
    // just a single child support
    donsus_typecheck_support_between_types(node->children[0]);

    assign_type_to_node(node->children[0], table, global_table);
    auto type_a = donsus_typecheck_type_expr(node->children[0]);

    node->get<donsus_ast::return_kw>().types.push_back(type_a);
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION: {
    donsus_typecheck_support_between_types(node->children[0]);
    assign_type_to_node(node->children[0], table, global_table);
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION: {
    // decide whether it's a string or single char
    node->real_type.type_un = DONSUS_TYPE::TYPE_STRING; // not entirely correct
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION: {
    node->real_type.type_un = DONSUS_TYPE::TYPE_BOOL;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
    for (auto &n : node->get<donsus_ast::function_def>().body) {
      if (n->type.type == donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT) {
        continue;
      }
      assign_type_to_node(n, table, global_table);
    }
    for (auto &param : node->get<donsus_ast::function_def>().parameters) {
      assign_type_to_node(param, table, global_table);
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_WHILE_LOOP: {
    for (auto &n : node->get<donsus_ast::while_loop>().body) {
      if (n->type.type == donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT) {
        continue;
      }
      assign_type_to_node(n, table, global_table);
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
    for (auto &param : node->get<donsus_ast::function_decl>().parameters) {
      assign_type_to_node(param, table, global_table);
    }

    node->real_type.type_un =
        node->get<donsus_ast::function_decl>().return_type[0].type_un;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FOR_LOOP: {
    for (auto &n : node->get<donsus_ast::for_loop>().body) {
      if (n->type.type == donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT) {
        continue;
      }
      assign_type_to_node(n, table, global_table);
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
    for (auto &n : node->get<donsus_ast::if_statement>().body) {
      if (n->type.type == donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT) {
        continue;
      }
      assign_type_to_node(n, table, global_table);
    }
    for (auto &n : node->get<donsus_ast::if_statement>().alternate) {
      if (n->type.type == donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT) {
        continue;
      }
      assign_type_to_node(n, table, global_table);
    }

    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT: {
    for (auto &n : node->get<donsus_ast::else_statement>().body) {
      if (n->type.type == donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT) {
        continue;
      }
      assign_type_to_node(n, table, global_table);
    }

    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {

    std::string func_name = node->get<donsus_ast::function_call>().func_name;
    std::string qualified_fn_name = global_table->apply_scope(func_name);

    if (global_table->sym_table.empty())
      donsus_type_error(node.get(), func_name + "has not been defined!");

    bool is_defined = donsus_is_function_exist(func_name, global_table);

    if (!is_defined)
      donsus_type_error(node.get(), func_name + "has not been defined!");

    utility::handle<DonsusSymTable> current_table =
        global_table->get_sym_table(qualified_fn_name);

    node->real_type.type_un = current_table->function_return_type[0].type_un;

    for (auto &args : node->get<donsus_ast::function_call>().arguments) {
      assign_type_to_node(args, table, global_table);
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION: {
    assign_type_to_node(node->children[0], table, global_table);
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER: {
    std::string iden_name = node->get<donsus_ast::identifier>().identifier_name;
    bool is_exist = donsus_sema_is_exist(iden_name, table);
    bool is_exist_global = donsus_sema_is_exist(iden_name, global_table);
    if (!is_exist && !is_exist_global)
      donsus_type_error(node.get(), iden_name + "is not defined!");

    DonsusSymTable::sym identifier_symbol_local = table->get(iden_name);
    DonsusSymTable::sym identifier_symbol_global = global_table->get(iden_name);

    if (identifier_symbol_local.mod != -1) {
      node->real_type = identifier_symbol_local.type;
    } else {
      node->real_type = identifier_symbol_global.type;
    }
    break;
  }

  default: {
  }
  }
}

DonsusSema::DonsusSema(DonsusAstFile &file,
                       utility::handle<donsus_ast::tree> tree)
    : file(&file), tree(tree) {
  tree->init_traverse();
}
void DonsusSema::start_traverse(utility::handle<DonsusSymTable> sym_global,
                                DonsusCodegen::DonsusCodeGenerator &codegen) {

  auto donsus_sym_call =
      std::bind(&DonsusSema::donsus_sym, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3);
  auto donsus_attn =
      std::bind(&DonsusSema::assign_type_to_node, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3);

  tree->traverse(donsus_sym_call, donsus_attn, sym_global, codegen);
}

void DonsusSema::start_traverse(utility::handle<DonsusSymTable> sym_global) {
  auto donsus_sym_call =
      std::bind(&DonsusSema::donsus_sym, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3);
  auto donsus_attn =
      std::bind(&DonsusSema::assign_type_to_node, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3);
  tree->traverse(donsus_sym_call, donsus_attn, sym_global);
}

auto DonsusSema::donsus_typecheck_is_valid_operator(donsus_token_kind kind)
    -> bool {
  switch (kind) {
  case DONSUS_PLUS:
  case DONSUS_MINUS:
  case DONSUS_STAR:
  case DONSUS_SLASH:
    return true;
  default: {
  }
  }
  return false;
}

auto DonsusSema::donsus_make_pos_from_token(donsus_token &token)
    -> donsus_token_pos {
  donsus_token_pos pos{};
  pos.line = token.line;
  pos.column = token.column;

  pos.abs_offset = token.offset;
  return pos;
}
auto DonsusSema::donsus_show_type_error_on_line(
    utility::handle<donsus_ast::node> node) {
  std::string value_c = file->parser->lexer.string;
  donsus_token ast_first_token = node.get()->start_offset_ast;
  donsus_token_pos ast_first_token_pos =
      donsus_make_pos_from_token(ast_first_token);

  donsus_token_pos end =
      donsus_make_pos_from_token(node.get()->get<donsus_token>());
  std::string whole_ast_range =
      value_c.substr(ast_first_token_pos.abs_offset, end.abs_offset);
  error.error_out_coloured(whole_ast_range, rang::fg::reset);
  error.error_out_coloured("\n");
  error.error_out_coloured("^", rang::fg::green);

  for (int i = 0; i < whole_ast_range.length(); i++) {
    error.error_out_coloured("~");
  }
  error.error_out_coloured("^", rang::fg::green);
  error.error_out_coloured("\n");
}

auto DonsusSema::donsus_type_error(utility::handle<donsus_ast::node> node,
                                   const std::string &message) -> void {
  if (!node) {
    file->error_count += 1;
    error.error_out_coloured("", rang::fg::reset);
    return;
  }
  error.error_out_coloured(message);
  donsus_show_type_error_on_line(node);
  error.error_out_coloured("", rang::fg::reset);
  file->error_count += 1;
}
auto DonsusSema::donsus_type_error(const std::string &message) -> void {
  file->error_count += 1;
  error.error_out_coloured(message);
  error.error_out_coloured("", rang::fg::reset);
}

// utility

//  Typechecking
auto DonsusSema::donsus_sym(utility::handle<donsus_ast::node> node,
                            utility::handle<DonsusSymTable> table,
                            utility::handle<DonsusSymTable> global_table)
    -> void {
  /*
   Call type checking function with the correct type
   **/
  switch (node->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    auto decl_name = node->get<donsus_ast::variable_decl>().identifier_name;

    bool is_declared = donsus_sema_is_duplicated(
        node->get<donsus_ast::variable_decl>().identifier_name, table, false);

    if (is_declared)
      donsus_type_error(node.get(), decl_name + "has been already declared!");
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_ARG: {
    auto arg_name = node->get<donsus_ast::variable_decl>().identifier_name;

    bool is_declared = donsus_sema_is_duplicated(
        node->get<donsus_ast::variable_decl>().identifier_name, table, false);

    if (is_declared)
      donsus_type_error(node.get(), arg_name + "has been already declared!");
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION: {
    auto decl_name = node->get<donsus_ast::array_decl>().identifier_name;

    bool is_declared = donsus_sema_is_duplicated(
        node->get<donsus_ast::array_decl>().identifier_name, table, false);

    if (is_declared)
      donsus_type_error(node.get(), decl_name + "has been already declared!");
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION: {
    auto def_name = node->get<donsus_ast::array_def>().identifier_name;

    bool is_declared = donsus_sema_is_duplicated(
        node->get<donsus_ast::array_def>().identifier_name, table, false);

    if (is_declared)
      donsus_type_error(node.get(), def_name + "has been already declared!!");

    // Match type of elements in array against the type of the array
    for (auto &n : node->get<donsus_ast::array_def>().elements) {
      donsus_typecheck_support_between_types(n);
      DONSUS_TYPE type_of_array_element = donsus_typecheck_type_expr(n);
      DONSUS_TYPE type_of_array =
          make_type(node->get<donsus_ast::array_def>().type); // type of array

      bool are_the_same =
          donsus_typecheck_is_compatible(type_of_array, type_of_array_element);
      if (!are_the_same) {
        donsus_type_error(node.get(),
                          "Operation between: " + type_of_array.to_string() +
                              "and " + type_of_array_element.to_string() +
                              "are not supported");
      }
    }
    // perform bound check if the array is:
    // fixed sized or static - this might be optional in the future
    if (node->get<donsus_ast::array_def>().number_of_elements >
            node->get<donsus_ast::array_def>().size &&
        (node->get<donsus_ast::array_def>().array_type ==
             donsus_ast::ArrayType::FIXED ||
         node->get<donsus_ast::array_def>().array_type ==
             donsus_ast::ArrayType::STATIC)) {
      donsus_type_error(
          node.get(),
          "array: " + node->get<donsus_ast::array_def>().identifier_name +
              " is out of bounds");
    }

    // combine this with out of bounds error
    if (node->get<donsus_ast::array_def>().number_of_elements !=
            node->get<donsus_ast::array_def>().size &&
        (node->get<donsus_ast::array_def>().array_type ==
             donsus_ast::ArrayType::FIXED ||
         node->get<donsus_ast::array_def>().array_type ==
             donsus_ast::ArrayType::STATIC)) {
      donsus_type_error(node.get(), "The size of the array must match with the "
                                    "one specified as the subscript");
    }
    // ensure the size is the same as number of elements
    // e.g dynamic array will have it when doing codegen
    node->get<donsus_ast::array_def>().size =
        node->get<donsus_ast::array_def>().number_of_elements;

    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
    // make sure it is not defined already
    // make sure the type is correct
    /*    sema.donsus_typecheck_assign_type_to_node(node);*/

    // ODR
    auto def_name = node->get<donsus_ast::variable_decl>().identifier_name;
    bool is_exist = donsus_sema_is_duplicated(def_name, table, false);
    bool is_exist_global =
        donsus_sema_is_duplicated(def_name, global_table, false);

    if (is_exist && is_exist_global)
      donsus_type_error(node.get(), def_name + "has been already defined!");

    // check for signed and unsigned
    if ((node->get<donsus_ast::variable_decl>().identifier_type == DONSUS_U32 ||
         node->get<donsus_ast::variable_decl>().identifier_type ==
             DONSUS_U64) &&
        node->children[0]->type.type ==
            donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION) {

      if (node->children[0]->children[0]->type.type ==
          donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION) {
        donsus_type_error(node.get(),
                          "Unsigned type cannot be negative at:" + def_name);
      }
    }

    assign_type_to_node(node->children[0], table, global_table);

    donsus_typecheck_support_between_types(node);

    DONSUS_TYPE type_of_var_def = donsus_typecheck_type_expr(node->children[0]);

    // type of the definition based on the signature
    auto local_type =
        make_type(node->get<donsus_ast::variable_decl>().identifier_type);

    bool is_compatible =
        donsus_typecheck_is_compatible(local_type, type_of_var_def);

    if (!is_compatible)
      donsus_type_error(
          node.get(), "Operation between: " + local_type.to_string() + " and " +
                          type_of_var_def.to_string() + " are not supported");
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
    // we can't assign types to an if statement
    // we can only assign types to its init statement condition
    // this is as of now not possible since this code runs before we would
    // assign types
    /*    sema.donsus_typecheck_assign_type_to_node(node);*/

    donsus_typecheck_type_is_bool_conversion(node->children[0]);
    node->children[0]->real_type.type_un = DONSUS_TYPE::kind::TYPE_BOOL;

    // see if the operations are supported
    donsus_typecheck_support_between_types(node->children[0]);

    if (!node->get<donsus_ast::if_statement>().body.empty()) {
      for (auto &children : node->get<donsus_ast::if_statement>().body) {
        donsus_sym(children, table, global_table);
      }
    }

    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_WHILE_LOOP: {
    donsus_typecheck_type_is_bool_conversion(node->children[0]);
    node->children[0]->real_type.type_un = DONSUS_TYPE::kind::TYPE_BOOL;

    // see if the operations are supported
    donsus_typecheck_support_between_types(node->children[0]);
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
    auto func_name = node->get<donsus_ast::function_decl>().func_name;
    bool is_func_declared = donsus_sema_is_duplicated(func_name, table, true);

    if (is_func_declared)
      donsus_type_error(node.get(), func_name + "has been already declared!");
    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
    auto func_name = node->get<donsus_ast::function_def>().func_name;
    bool is_defined = donsus_sema_is_duplicated(func_name, table, true);

    if (is_defined)
      donsus_type_error(node.get(), func_name + "has bene already declared!");

    // loop through the function, assign types and then come back
    donsus_typecheck_is_return_type_valid(node);
    // need to call this later
    // check for function def specific stuff then just recursion
    if (!node->get<donsus_ast::function_def>().body.empty()) {
      for (auto &children : node->get<donsus_ast::function_def>().body) {

        donsus_sym(children, table, global_table);
      }
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT: {
    // auto assignment_name =
    // node->get<donsus_ast::assignment>().identifier_name;

    // bool is_defined = sema.donsus_sema_is_exist(assignment_name, table);

    // if (!is_defined)
    //   throw DonsusUndefinedException(assignment_name + " is not defined");

    assign_type_to_node(node->get<donsus_ast::assignment>().lvalue, table,
                        global_table);
    assign_type_to_node(node->get<donsus_ast::assignment>().rvalue, table,
                        global_table);

    donsus_typecheck_support_between_types(
        node->get<donsus_ast::assignment>().lvalue);

    donsus_typecheck_support_between_types(
        node->get<donsus_ast::assignment>().rvalue);

    bool are_the_same = donsus_typecheck_is_compatible(
        node->get<donsus_ast::assignment>().lvalue->real_type,
        node->get<donsus_ast::assignment>().rvalue->real_type);
    if (!are_the_same) {
      donsus_type_error(node.get(), "Operation between: " +
                                        node->get<donsus_ast::assignment>()
                                            .lvalue->real_type.to_string() +
                                        " and " +
                                        node->get<donsus_ast::assignment>()
                                            .rvalue->real_type.to_string() +
                                        " are not supported");
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER: {

    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {
    std::string func_name = node->get<donsus_ast::function_call>().func_name;

    if (!table->get_sym_table_from_unqualified(func_name)) {
      donsus_type_error(node.get(), func_name + " has not been defined!");
    }

    utility::handle<DonsusSymTable> current_table =
        table->get_sym_table_from_unqualified(func_name);

    // Check whether the number of arguments is correct
    unsigned int number_of_args =
        node->get<donsus_ast::function_call>().arguments.size();
    int number_of_args_defined = current_table->get_function_argument_size();

    if (number_of_args != number_of_args_defined)
      donsus_type_error(node.get(),
                        "Number of arguments in function call: " + func_name +
                            " is not correct, " +
                            std::to_string(number_of_args) + "were provided " +
                            std::to_string(number_of_args_defined) +
                            "were defined");

    for (int i = 0; i < node->get<donsus_ast::function_call>().arguments.size();
         ++i) {
      DONSUS_TYPE function_arg = current_table->get_function_argument(i);
      DONSUS_TYPE function_call_arg =
          node->get<donsus_ast::function_call>().arguments[i]->real_type;
      bool compatible =
          donsus_typecheck_is_compatible(function_arg, function_call_arg);
      if (!compatible)
        donsus_type_error(
            node.get(),
            "type: " + current_table->get_function_argument(i).to_string() +
                " is not compatible with type: " +
                node->get<donsus_ast::function_call>()
                    .arguments[i]
                    ->real_type.to_string() +
                " in function: '" + func_name + "'");
    }

    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT: {
    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
    // assign type
    /*    sema.donsus_typecheck_assign_type_to_node(node->children[0]);*/
    return;
  }
  default: {
  }
  }
}

// check if its duplicated(true)
auto DonsusSema::donsus_sema_is_duplicated(
    std::string &name, utility::handle<DonsusSymTable> table, bool for_function)
    -> bool {
  // check if the
  // for_function is a boolean that is true when we are checking for a function
  // we have to use get_sym_table when we are checking for a function
  if (for_function) {
    std::string qu_name = table->apply_scope(name);
    utility::handle<DonsusSymTable> result = table->get_sym_table(qu_name);
    if (result && result->duplicated)
      return true;
    return false;
  } else {
    DonsusSymTable::sym result = table->get(name);
    if (result.duplicated)
      return true;
    return false;
  }
}

auto DonsusSema::donsus_sema_is_exist(std::string &name,
                                      utility::handle<DonsusSymTable> table)
    -> bool {
  DonsusSymTable::sym result = table->get(name);
  return result.mod != -1;
}

auto DonsusSema::donsus_is_function_exist(std::string &name,
                                          utility::handle<DonsusSymTable> table)
    -> bool {
  // assuming that we are in the table of the function
  // TODO: make sure this works with multiple functions
  // here we should start with global
  std::string qu_name = table->apply_scope(name);

  bool is_exists = table->is_sym_table_exist(qu_name);
  return is_exists;
}

/**
 * \brief Checks if the 2 types are compatible.
 */
auto DonsusSema::donsus_typecheck_is_compatible(DONSUS_TYPE first,
                                                DONSUS_TYPE second) -> bool {

  // call == overload
  return (first == second);
}

/**
 * \brief Checks if the type of the node is contextually bool convertable.
 */
auto DonsusSema::donsus_typecheck_type_is_bool_conversion(
    utility::handle<donsus_ast::node> node) -> bool {
  switch (node->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL:
  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER:
  case donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION:
    return true;

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION:
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF:
  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT:
  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT:
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT:
  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT:
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION:
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_ARG:
    return false;
  }
}

/**
 * \brief Returns the type of the expr.
 */
auto DonsusSema::donsus_typecheck_type_expr(
    utility::handle<donsus_ast::node> node) -> DONSUS_TYPE {

  if (node->type.type == donsus_ast::donsus_node_type::DONSUS_EXPRESSION ||
      node->type.type ==
          donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION) {
    for (auto n : node->children) {
      if (n->type.type != donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
        return n->real_type;
      }
    }
  }
  return node->real_type;
}

/**
 * \brief Check if the operators are supported between operands.
 */
void DonsusSema::donsus_typecheck_support_between_types(
    utility::handle<donsus_ast::node> node, std::set<DONSUS_TYPE> *childTypes) {

  std::set<DONSUS_TYPE> defaultSet;
  if (childTypes == nullptr) {
    childTypes = &defaultSet;
  }

  if (!node)
    return;

  if (node->type.type == donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
    // If the node is an expression, recursively compare types of its children

    for (auto &child : node->children) {
      if (child->type.type != donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
        childTypes->insert(child->real_type);
      }
      donsus_typecheck_support_between_types(child, childTypes);
    }

    // Check for inconsistent types within the expression
    if (childTypes->size() > 1) {
      std::string types{};
      for (auto &type : *childTypes) {
        types += " " + type.to_string() + ",";
      }
      donsus_type_error(node.get(),
                        "Inconsistent types in expression between types:" +
                            types);
    }
  }
}

auto DonsusSema::donsus_typecheck_is_return_type_valid(
    utility::handle<donsus_ast::node> node) -> void {
  std::vector<DONSUS_TYPE> expect =
      node->get<donsus_ast::function_def>().return_type;
  int found = 0;

  for (auto n : node->get<donsus_ast::function_def>().body) {
    donsus_typecheck_is_return_type_valid_for_children(
        n, node->get<donsus_ast::function_def>().return_type, found);
    if (n->type.type == donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT) {
      for (auto n : n->get<donsus_ast::if_statement>().body) {
        donsus_typecheck_is_return_type_valid_for_children(
            n, node->get<donsus_ast::function_def>().return_type, found);
      }
      for (auto n : n->get<donsus_ast::if_statement>().alternate) {
        donsus_typecheck_is_return_type_valid_for_children(
            n, node->get<donsus_ast::function_def>().return_type, found);
      }
    }
  }
  // if its void just terminate
  if (expect[0].type_un == DONSUS_TYPE::TYPE_VOID) {
    return;
  }
  if (found == 0) {
    donsus_type_error(node.get(), "Return statement is not correct, TBD!");
  }
}

auto DonsusSema::donsus_typecheck_is_return_type_valid_for_children(
    utility::handle<donsus_ast::node> n, std::vector<DONSUS_TYPE> expect,
    int &found) -> void {
  if (n->type.type == donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT) {
    // examine it here
    // if the type is void
    if (expect[0].type_un == DONSUS_TYPE::TYPE_VOID) {
      donsus_type_error("Return is not possible when  type is void");
    }
    if (n->get<donsus_ast::return_kw>().types == expect)
      found++;
    // multiple return types
    if (expect.size() > 1) {
      for (auto type : expect) {
        for (auto type2 : n->get<donsus_ast::return_kw>().types) {
          if (type != type2) {
            donsus_type_error("Return is not correct!");
          }
        }
      }
    }
  }
}