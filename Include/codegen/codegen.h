#include "../parser.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <memory>

#ifndef CODEGEN_H
#define CODEGEN_H

namespace DonsusCodegen {

class DonsusCodeGenerator {
  // destination driven code generating system
public:
  void compile_donsus_expr(DonsusParser::end_result &ast);
  void compile(DonsusParser::end_result &ast);

  // visit functions, emit IR
  llvm::Value *visit(donsus_ast::variable_decl &ast);
  llvm::Value *visit(donsus_ast::assignment &ast);
  llvm::Value *visit(donsus_ast::identifier &ast);
  llvm::Value *visit(donsus_ast::number_expr &ast);
  llvm::Value *visit(donsus_ast::function_decl &ast);
  llvm::Value *visit(donsus_ast::function_def &ast);
  llvm::Value *visit(donsus_ast::function_call &ast);
  llvm::Value *visit(donsus_ast::if_statement &ast);
  llvm::Value *visit(donsus_ast::else_statement &ast);
  llvm::Value *visit(donsus_ast::return_kw &ast);
  llvm::Value *visit(donsus_ast::string_expr &ast);
  llvm::Value *visit(donsus_ast::expression &ast);
  llvm::Value *visit(donsus_ast::print_expr &ast);

private:
  // data members
  static std::unique_ptr<LLVMContext> TheContext;
  static std::unique_ptr<IRBuilder<>> Builder(TheContext);
  static std::unique_ptr<Module> TheModule;
};

} // namespace DonsusCodegen
#endif