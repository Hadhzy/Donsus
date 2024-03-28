/*
 Codegen for the DONSUS COMPILER
Todo:
- build vtable: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable
- visit each of the ast nodes
- optimisation is not needed as the IR builder already constant one
- IRBuilder
- assign them in symbol table
 // signed support
 // unsigned support
 */
#include "../../Include/codegen/codegen.h"

namespace DonsusCodegen {
// traverse
void DonsusCodeGenerator::compile(utility::handle<donsus_ast::node> &n,
                                  utility::handle<DonsusSymTable> &table) {
  /*
  we either pass in the actual ast structure or the high level node including
  children
   * */
  switch (n->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    visit(n->get<donsus_ast::variable_decl>());
    break;
  }
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
    visit(n->get<donsus_ast::variable_decl>(), true);
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
    visit(n);
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

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::variable_decl &ast,
                                        bool is_definition) {
  // allocate variable on the stack
  // use the one from the tutorial

  // variable definition
  if (is_definition) {
  }

  // variable declaration
  auto type = ast.identifier_type;
  auto name = ast.identifier_name;

  llvm::AllocaInst *Alloca =
      Builder->CreateAlloca(map_type(make_type(type)), 0, name);
  // add it to the specific symbol
}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::assignment &ast) {}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast) {
  // arbitrary precision integer, typically common unsigned ints
  // context_, llvm:PInt(32, int_expr.GetValAsInt(), true)));
  // only works with scalar integers, constant propagation is needed
  // constant propagate the operands into an unsigned int
  int value = ast->constant_propagation(ast);

  bool is_signed = false;
  if (value < 0) {
    is_signed = true;
  }

  return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, value, is_signed));
}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::identifier &ast) {}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::function_decl &ast) {}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::function_def &ast) {}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::function_call &ast) {}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::if_statement &ast) {}

llvm::Value *DonsusCodeGenerator ::visit(donsus_ast::else_statement &ast) {}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::return_kw &ast) {}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::string_expr &ast) {
  // first immutable
  // make a new global variable, i8*

  Builder->CreateGlobalStringPtr(llvm::StringRef(ast.value.value));
}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::expression &ast) {}

llvm::Value *DonsusCodeGenerator::visit(donsus_ast::print_expr &ast) {}

/*Maps DONSUS_TYPE to llvm TYPE.
 **/
llvm::Type *DonsusCodegen::DonsusCodeGenerator::map_type(DONSUS_TYPE type) {
  switch (type.type_un) {
  case DONSUS_TYPE::TYPE_VOID: {
  }

  // integers
  case DONSUS_TYPE::TYPE_I8: {
    return llvm::IntegerType::get(*TheContext, 8);
  }

  case DONSUS_TYPE::TYPE_I16: {
    return llvm::IntegerType::get(*TheContext, 16);
  }

  case DONSUS_TYPE::TYPE_BASIC_INT: {
    // arbitrary precision integer, like BIGINT
  }

  case DONSUS_TYPE::TYPE_I32: {
    return llvm::IntegerType::get(*TheContext, 32);
  }

  case DONSUS_TYPE::TYPE_I64: {
    return llvm::IntegerType::get(*TheContext, 64);
  }
  case DONSUS_TYPE::TYPE_U32: {
    break;
  }

  case DONSUS_TYPE::TYPE_U64: {
    break;
  }

  case DONSUS_TYPE::TYPE_CHAR: {
    break;
  }
  default: {
  }
  }
}
} // namespace DonsusCodegen
