#ifndef CODEGEN_H
#define CODEGEN_H

#include "../parser.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <memory>

namespace DonsusCodegen {

class DonsusCodeGenerator {
  // destination driven code generating system
public:
  void compile_donsus_expr(DonsusParser::end_result &ast);
  void compile(utility::handle<donsus_ast::node> &node,
               utility::handle<DonsusSymTable> &table);

  /// CreateEntryBlockAlloca - Create an alloca instruction in the entry block
  /// of the function.  This is used for mutable variables etc.
  // visit functions, emit IR
  llvm::Value *visit(donsus_ast::variable_decl &ast,
                     bool is_definition = false);
  llvm::Value *visit(donsus_ast::assignment &ast);
  llvm::Value *visit(donsus_ast::identifier &ast);
  llvm::Value *visit(utility::handle<donsus_ast::node> &ast);
  llvm::Value *visit(donsus_ast::function_decl &ast);
  llvm::Value *visit(donsus_ast::function_def &ast);
  llvm::Value *visit(donsus_ast::function_call &ast);
  llvm::Value *visit(donsus_ast::if_statement &ast);
  llvm::Value *visit(donsus_ast::else_statement &ast);
  llvm::Value *visit(donsus_ast::return_kw &ast);
  llvm::Value *visit(donsus_ast::string_expr &ast);
  llvm::Value *visit(donsus_ast::expression &ast);
  llvm::Value *visit(donsus_ast::print_expr &ast);

  llvm::Type *map_type(DONSUS_TYPE type);

private:
  // data members
  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> TheModule;
  // assigning INST's during codegen
  // DonsusSymTable TheSymbolTable;

  // debug
};

} // namespace DonsusCodegen
#endif