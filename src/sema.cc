#include "../Include/sema.h"

DonsusSema sema;
auto assign_type_to_node(utility::handle<donsus_ast::node> node,
                         utility::handle<DonsusSymTable> table,
                         utility::handle<DonsusSymTable> global_table) -> void;

auto process_donsus_expression(utility::handle<donsus_ast::node> node,
                               utility::handle<DonsusSymTable> table,
                               utility::handle<DonsusSymTable> global_table)
    -> void {
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
auto decide_type_for_expression(utility::handle<donsus_ast::node> node,
                                utility::handle<DonsusSymTable> table)
    -> DONSUS_TYPE {
  for (auto n : node->children) {
    if (n->type.type != donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
      return n->real_type;
    }
  }
}

auto assign_type_to_node(utility::handle<donsus_ast::node> node,
                         utility::handle<DonsusSymTable> table,
                         utility::handle<DonsusSymTable> global_table) -> void {
  switch (node->type.type) {

  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
    // we can't figure out the type here.
    // should be compatible with other types
    node->real_type.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
    break;
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

  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
    process_donsus_expression(node, table, global_table);
    sema.donsus_typecheck_support_between_types(node);
    // after all the children of donsus expression got a type
    // we can just pick the closest one and set it up as the expression
    // type
    node->real_type.type_un = decide_type_for_expression(node, table).type_un;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
    // just a single child support
    sema.donsus_typecheck_support_between_types(node->children[0]);

    assign_type_to_node(node->children[0], table, global_table);
    auto type_a = sema.donsus_typecheck_type_expr(node->children[0]);

    node->get<donsus_ast::return_kw>().types.push_back(type_a);
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION: {
    sema.donsus_typecheck_support_between_types(node->children[0]);
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
      throw ReDefinitionException(func_name + " has not been defined!");

    bool is_defined = sema.donsus_is_function_exist(func_name, global_table);

    if (!is_defined)
      throw ReDefinitionException(func_name + " has not been defined!");

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
    bool is_exist = sema.donsus_sema_is_exist(iden_name, table);
    if (!is_exist)
      throw DonsusUndefinedException(iden_name + " is not defined");

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

// utility

//  Typechecking
void donsus_sym(utility::handle<donsus_ast::node> node,
                utility::handle<DonsusSymTable> table,
                utility::handle<DonsusSymTable> global_table) {
  /*
   Call type checking function with the correct type
   **/
  switch (node->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    auto decl_name = node->get<donsus_ast::variable_decl>().identifier_name;

    bool is_declared = sema.donsus_sema_is_duplicated(
        node->get<donsus_ast::variable_decl>().identifier_name, table);

    if (is_declared)
      throw ReDefinitionException(decl_name + " has been already declared!");
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
    // make sure it is not defined already
    // make sure the type is correct
    /*    sema.donsus_typecheck_assign_type_to_node(node);*/

    // ODR
    auto def_name = node->get<donsus_ast::variable_decl>().identifier_name;
    bool is_exist = sema.donsus_sema_is_duplicated(def_name, table);

    if (is_exist)
      throw ReDefinitionException(def_name + " has been already defined!");

    // check for signed and unsigned
    if ((node->get<donsus_ast::variable_decl>().identifier_type == DONSUS_U32 ||
         node->get<donsus_ast::variable_decl>().identifier_type ==
             DONSUS_U64) &&
        node->children[0]->type.type ==
            donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION) {

      if (node->children[0]->children[0]->type.type ==
          donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION) {
        throw UnsignedTypeException("Unsigned type cannot be negative at: " +
                                    def_name);
      }
    }

    assign_type_to_node(node->children[0], table, global_table);

    sema.donsus_typecheck_support_between_types(node);

    DONSUS_TYPE type_of_var_def =
        sema.donsus_typecheck_type_expr(node->children[0]);

    // type of the definition based on the signature
    auto local_type =
        make_type(node->get<donsus_ast::variable_decl>().identifier_type);

    bool is_compatible =
        sema.donsus_typecheck_is_compatible(local_type, type_of_var_def);

    if (!is_compatible)
      throw InCompatibleTypeException(
          "Operation between: " + local_type.to_string() + " and " +
          type_of_var_def.to_string() + " are not supported");
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
    // we can't assign types to an if statement
    // we can only assign types to its init statement condition
    // this is as of now not possible since this code runs before we would
    // assign types
    /*    sema.donsus_typecheck_assign_type_to_node(node);*/

    sema.donsus_typecheck_type_is_bool_conversion(node->children[0]);
    node->children[0]->real_type.type_un = DONSUS_TYPE::kind::TYPE_BOOL;

    // see if the operations are supported
    sema.donsus_typecheck_support_between_types(node->children[0]);

    if (node->get<donsus_ast::if_statement>().body.size() != 0) {
      for (auto &children : node->get<donsus_ast::if_statement>().body) {
        donsus_sym(children, table, global_table);
      }
    }

    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
    auto func_name = node->get<donsus_ast::function_decl>().func_name;
    bool is_func_declared = sema.donsus_sema_is_duplicated(func_name, table);

    if (is_func_declared)
      throw ReDefinitionException(func_name + "has been already declared");

    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
    auto func_name = node->get<donsus_ast::function_def>().func_name;
    bool is_defined = sema.donsus_sema_is_duplicated(func_name, table);

    if (is_defined)
      throw ReDefinitionException(func_name + "has been already declared");

    // loop through the function, assign types and then come back
    // need to call this later
    sema.donsus_typecheck_is_return_type_valid(node);
    // check for function def specific stuff then just recursion
    if (node->get<donsus_ast::function_def>().body.size() != 0) {
      for (auto &children : node->get<donsus_ast::function_def>().body) {
        donsus_sym(children, table, global_table);
      }
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT: {
    auto assignment_name = node->get<donsus_ast::assignment>().identifier_name;

    bool is_defined = sema.donsus_sema_is_exist(assignment_name, table);

    if (!is_defined)
      throw DonsusUndefinedException(assignment_name + " is not defined");

    assign_type_to_node(node->children[0], table, global_table);

    sema.donsus_typecheck_support_between_types(node->children[0]);
    DONSUS_TYPE rvalue_expression_type =
        sema.donsus_typecheck_type_expr(node->children[0]);
    DONSUS_TYPE assigned_value_type = table->get(assignment_name).type;

    bool are_the_same = sema.donsus_typecheck_is_compatible(
        assigned_value_type, rvalue_expression_type);
    if (!are_the_same) {
      throw InCompatibleTypeException(
          "Operation between: " + assigned_value_type.to_string() + " and " +
          rvalue_expression_type.to_string() + " are not supported");
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

    bool is_defined = sema.donsus_is_function_exist(func_name, table);
    if (!is_defined)
      throw ReDefinitionException(func_name + " has not been defined!");
    std::string qualified_fn_name = table->apply_scope(func_name);
    if (table->sym_table.size() == 0)
      throw ReDefinitionException(func_name + " has not been defined!");
    utility::handle<DonsusSymTable> current_table =
        table->get_sym_table(qualified_fn_name);

    int i = 0;
    for (i = 0; i < node->get<donsus_ast::function_call>().arguments.size();
         ++i) {
      DONSUS_TYPE function_arg = current_table->get_function_argument(i);
      DONSUS_TYPE function_call_arg =
          node->get<donsus_ast::function_call>().arguments[i]->real_type;
      bool compatible =
          sema.donsus_typecheck_is_compatible(function_arg, function_call_arg);
      if (!compatible)
        throw InCompatibleTypeException(
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
    std::string &name, utility::handle<DonsusSymTable> table) -> bool {
  // check if the
  DonsusSymTable::sym result = table->get(name);
  if (result.duplicated)
    return true;
  return false;
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
      throw InCompatibleTypeException(
          "Inconsistent types in expression between types:" + types);
    }
  }
}

auto DonsusSema::donsus_typecheck_is_return_type_valid(
    utility::handle<donsus_ast::node> node) -> void {
  std::vector<DONSUS_TYPE> expect =
      node->get<donsus_ast::function_def>().return_type;

  for (auto n : node->get<donsus_ast::function_def>().body) {
    if (n->type.type == donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT) {
      // examine it here
      // if the type is void
      if (expect[0].type_un == DONSUS_TYPE::TYPE_VOID) {
        throw ReturnTypeException("Return is not possible when  type is void");
      }
      if (n->get<donsus_ast::return_kw>().types == expect)
        return;
      // multiple return types
      if (expect.size() > 1) {
        for (auto type : expect) {
          for (auto type2 : n->get<donsus_ast::return_kw>().types) {
            if (type != type2) {
              throw ReturnTypeException("Return is not correct");
            }
          }
        }
        return;
      }
    }
  }
  // if its void just terminate
  if (expect[0].type_un == DONSUS_TYPE::TYPE_VOID) {
    return;
  }
  throw ReturnTypeException("Return statement is not correct, TBD!");
}