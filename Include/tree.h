// AST TREE API FOR PARSER.CC
#ifndef DONSUS_TREE_H
#define DONSUS_TREE_H
#include <memory>
#include "parser.h"
#include <iostream>


std::unique_ptr<donsus_ast> donsus_make_ast_node(donsus_token value, std::unique_ptr<donsus_ast> left, std::unique_ptr<donsus_ast> right);
std::unique_ptr<donsus_ast> donsus_make_ast_leaf(donsus_token value);
std::unique_ptr<donsus_ast> donsus_make_ast_unary(donsus_token value, std::unique_ptr<donsus_ast> left);
#endif // !DONSUS_TREE_H
