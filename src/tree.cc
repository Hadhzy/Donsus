// AST TREE API FOR PARSER.CC wrapper around donsus_ast
#include "../Include/parser.h"
#include <memory>

std::unique_ptr<donsus_ast>
donsus_make_ast_node(donsus_token &value, std::unique_ptr<donsus_ast> left,
                     std::unique_ptr<donsus_ast> right,
                     donsus_token_kind type) {
  std::unique_ptr<donsus_ast> n = std::make_unique<donsus_ast>();

  n->value = value;
  n->left = std::move(left);
  n->right = std::move(right);
  n->type = type;
  return n;
}

std::unique_ptr<donsus_ast> donsus_make_ast_leaf(donsus_token &value,
                                                 donsus_token_kind type) {
  return donsus_make_ast_node(value, nullptr, nullptr, type);
}

std::unique_ptr<donsus_ast>
donsus_make_ast_unary(donsus_token &value, std::unique_ptr<donsus_ast> left,
                      donsus_token_kind type) {
  return donsus_make_ast_node(value, std::move(left), nullptr, type);
}
