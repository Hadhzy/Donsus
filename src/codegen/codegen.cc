/*
 Codegen for the DONSUS COMPILER
Todo:
- build vtable: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable
- visit each of the ast nodes
- optimisation is not needed as the IR builder already constant one
- IRBuilder

 */
#include "../../Include/codegen/codegen.h"

// traverse
void DonsusCodegen::DonsusCodeGenerator::compile(
    DonsusParser::end_result &ast) {

  for (auto &n : ast->get_nodes()) {
    switch (n->type.type) {
    case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
      visit(n->get<donsus_ast::variable_decl>());
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
      visit(n->get<donsus_ast::variable_decl>());
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
      visit(n->get<donsus_ast::function_decl>());
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
      visit(n->get<donsus_ast::function_def>());
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
      visit(n->get<donsus_ast::if_statement>());
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT: {
      visit(n->get<donsus_ast::assignment>());
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER: {
      visit(n->get<donsus_ast::identifier>());
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
      visit(n->get<donsus_ast::number_expr>());
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
      visit(n->get<donsus_ast::expression>());
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {
      visit(n->get<donsus_ast::function_call>());
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT: {
      visit(n->get<donsus_ast::else_statement>());
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
      visit(n->get<donsus_ast::return_kw>());
      break;
    }
    case donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION: {
      visit(n->get<donsus_ast::string_expr>());
      break;
    }

    case donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION: {
      visit(n->get<donsus_ast::print_expr>());
      break;
    }
    }
  }
}
llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::variable_decl &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::assignment &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::number_expr &ast){
    // arbitrary precision integer, typically common unsigned ints
    // context_, llvm::APInt(32, int_expr.GetValAsInt(), true)));
    // only works with scalar integers, constant propagation is needed
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, ast.value))}

llvm::Value *DonsusCodegen::DonsusCodeGenerator::visit(
    donsus_ast::identifier &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::function_decl &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::function_def &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::function_call &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::if_statement &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator ::visit(donsus_ast::else_statement &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::return_kw &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::string_expr &ast){
    // first immutable
    // make a new global variable, i8*

    Builder.CreateGLobalStringPtr(StrinRef(ast.value.value))}

llvm::Value *DonsusCodegen::DonsusCodeGenerator::visit(
    donsus_ast::expression &ast) {}

llvm::Value *
DonsusCodegen::DonsusCodeGenerator::visit(donsus_ast::print_expr &ast) {}