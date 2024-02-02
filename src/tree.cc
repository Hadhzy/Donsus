#include "../Include/parser.h"

// Functions for generating the AST structure


// Construct an AST node and return it
donsus_ast& make_ast_node(donsus_token value, donsus_token op, donsus_ast *left, donsus_ast *right) {
    donsus_ast n;

    // TODO: Proper memory allocation

    n.value = value;
    n.op = op;
    n.left = left;
    n.right = right;

    return n;
}

// Creates a leaf AST node, meaning that without 
donsus_ast& make_ast_leaf(donsus_token op, donsus_token value) {
    return (make_ast_node(value, op, NULL, NULL));
}

// Construct a unary which only has one side
donsus_ast& make_ast_unary(donsus_token op, donsus_ast *left, donsus_token value) {
    return (make_ast_node(value, op, left, NULL));
}