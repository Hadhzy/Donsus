// Construct symbol table from ast(1)
// Semantic analysis generates SEA OF NODES from symbol table(2) or AST(TBD)
/* \*
 *    AST(no):
 *      +
 *     /  \
 *    2    3
 *
 *
 *      name     type    scope
 *     |-----------------------|
 *     |       |        |      |
 *     |-----------------------|
 *     |       |        |      |
 *     |-----------------------|
 *     |       |        |      |
 *     |-----------------------|
 *
 */
/*
 Type checking TODOS:
  - check if the number of return values matches the one in the function
 prototype
  unsigned long int a = 12 + "sdfsd" + func_call();
  - figure out if the if statement is true

  -see whether parameters/arguments have the correct type.
  - typecheck assignments

DONSUS_EXPRESSION:
  Call donsus_typecheck_support_between_types to see whether operators between
operands are supported
  - Assign type of DONSUS_EXPRESSION by assigning type to each of its nodes,
  and then pick the closest one(from children - process_donsus_expression) to
determine the expression type -Obtaining the type happens in
donsus_typecheck_type_expr.
  - Then just simply match it against the type defined in the variable
definition a:int - here int is the type.
  */

#include "../Include/sema.h"

DonsusSema sema;
auto assign_type_to_node(utility::handle<donsus_ast::node> node) -> void;

auto process_donsus_expression(utility::handle<donsus_ast::node> node) -> void {
  for (auto n : node->children) {
    if (n->type.type == donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
      process_donsus_expression(n);
    } else {
      assign_type_to_node(n);
    }
  }
  // go until find a proper type
}

auto assign_type_to_node(utility::handle<donsus_ast::node> node) -> void {
  switch (node->type.type) {

  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
    // we can't figure out the type here.
    // should be compatible with other types
    node->real_type.type_un = DONSUS_TYPE::TYPE_BASIC_INT;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {

    node->real_type.type_un =
        make_type(node->get<donsus_ast::variable_decl>().identifier_type)
            .type_un;
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
    process_donsus_expression(node);
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
    // just a single child support
    sema.donsus_typecheck_support_between_types(node->children[0]);

    assign_type_to_node(node->children[0]);
    auto type_a = sema.donsus_typecheck_type_expr(node->children[0]);

    node->get<donsus_ast::return_kw>().types.push_back(type_a);
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION: {
    // decide whether it's a string or single char
    node->real_type.type_un = DONSUS_TYPE::TYPE_CHAR; // not entirely correct
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {
    for (auto &args : node->get<donsus_ast::function_call>().arguments) {
      assign_type_to_node(args);
    }
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
                utility::handle<DonsusSymTable> table) {
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

    utility::handle<donsus_ast::node> a =
        sema.donsus_typecheck_support_between_types(node);

    assign_type_to_node(node->children[0]);

    DONSUS_TYPE type_of_var_def =
        sema.donsus_typecheck_type_expr(node->children[0]);

    // type of the definition based on the signature
    auto local_type =
        make_type(node->get<donsus_ast::variable_decl>().identifier_type);

    bool is_compatible =
        sema.donsus_typecheck_is_compatible(local_type, type_of_var_def);

    if (!is_compatible)
      throw InCompatibleTypeException(
          "Operation between: " + local_type.to_string() + " and" +
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
        donsus_sym(children, table);
      }
    }
    break;
  }

  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT: {
    auto assignment_name = node->get<donsus_ast::assignment>().identifier_name;

    bool is_defined = sema.donsus_sema_is_exist(assignment_name, table);

    if (!is_defined)
      throw DonsusUndefinedException(assignment_name + " is not defined");

    sema.donsus_typecheck_support_between_types(node);
    DONSUS_TYPE t = sema.donsus_typecheck_type_expr(node->children[0]);

    bool are_the_same = sema.donsus_typecheck_is_compatible(
        table->get(assignment_name).type, t);
    if (!are_the_same) {
      throw InCompatibleTypeException(
          "Operation between: " + table->get(assignment_name).type.to_string() +
          " and " + t.to_string() + " are not supported");
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

void DonsusSema::donsus_sema(utility::handle<donsus_ast::node> ast) {
  // Entry Point
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
  if (result.mod != -1)
    return true;
  return false;
}

auto DonsusSema::donsus_is_function_exist(std::string &name,
                                          utility::handle<DonsusSymTable> table)
    -> bool {
  // assuming that we are in the table of the function
  // TODO: make sure this works with multiple functions
  std::string qu_name = table->apply_scope(name);
  bool is_exists = table->is_sym_table_exist(qu_name, table);
  if (!is_exists) {
    return false;
  } else {
    return true;
  }
};

/**
 * \brief Checks if the 2 types are compatible.
 */
auto DonsusSema::donsus_typecheck_is_compatible(DONSUS_TYPE first,
                                                DONSUS_TYPE second) -> bool {

  // call == overload
  if (first == second)
    return true;
  return false;
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

    return true;

  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION:
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF:
  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT:
  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT:
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT:
    return false;
  }
}

/**
 * \brief Returns the type of the expr.
 */
auto DonsusSema::donsus_typecheck_type_expr(
    utility::handle<donsus_ast::node> node) -> DONSUS_TYPE {

  if (node->type.type == donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
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
auto DonsusSema::donsus_typecheck_support_between_types(
    // get the highest node
    // see if its children are compatible move to next children
    // int a = 10;
    utility::handle<donsus_ast::node> node, int level)
    -> utility::handle<donsus_ast::node> {

  utility::handle<donsus_ast::node> lhs;
  utility::handle<donsus_ast::node> rhs;

  if (node->children.empty()) {
    // scalar
    return node;
  }

  if (!node->children[0]) {
    // stop recursion
    return node;
  }

  node = node->children[0];
  // if node operator call with left and right
  if (node->type.type == donsus_ast::donsus_node_type::DONSUS_EXPRESSION) {
    lhs = node->children[0];
    rhs = node->children[1];
  } else {
    return node;
  }

  level++;
  lhs = donsus_typecheck_support_between_types(lhs, level);
  rhs = donsus_typecheck_support_between_types(rhs, level);

  auto is_compatible =
      donsus_typecheck_is_compatible(lhs->real_type, rhs->real_type);

  if (!is_compatible)
    throw InCompatibleTypeException(
        "Operation between: " + lhs->real_type.to_string() +
        " and:" + rhs->real_type.to_string() + " are not supported");

  return node;
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
    }
  }
  // if its void just terminate
  if (expect[0].type_un == DONSUS_TYPE::TYPE_VOID) {
    return;
  }
  throw ReturnTypeException("Return statement is not correct, TBD!");
}