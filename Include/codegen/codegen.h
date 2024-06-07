#ifndef DONSUS_CODEGEN_H
#define DONSUS_CODEGEN_H

#include "../parser.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
// opt
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

#include <memory>
#include <iostream>
#include <filesystem>
#include "../../Include/Internal/build_context.h"

// Select platform
#if defined(DU_SYSTEMS_WINDOWS)
#include "../../src/codegen/platform/windows_platform.h"
using PlatformClass = WindowsPlatform;
#endif

#if defined(DU_SYSTEMS_UNIX)
#include "../../src/codegen/platform/linux_platform.h"
using PlatformClass = LinuxPlatform;
#endif

#if defined(DU_SYSTEMS_OSX)
#include "../../src/codegen/platform/osx_platform.h"
#endif

namespace DonsusCodegen 
{
Bitness GetBitness();
class Platform {
public:
  std::string
  GetLinkerCommand(const std::vector<std::filesystem::path> &obj_paths,
                   const std::filesystem::path &exe_path,
                   Bitness bitness) const {

    return obj.GetLinkerCommand(obj_paths, exe_path, bitness);
  }
  PlatformClass obj;
};
class DonsusCodeGenerator {
public:
  DonsusCodeGenerator() = default;
  DonsusCodeGenerator(std::unique_ptr<llvm::LLVMContext> context,
                      std::unique_ptr<llvm::Module> module,
                      std::unique_ptr<llvm::IRBuilder<>> builder);

  void Finish() const;

  void Link() const;

  int create_object_file();

  void load_built_in();

  // run llvm's default optimisation pipeline
  void default_optimisation();

  void create_entry_point();

  // helper functions
  llvm::StructType multiple_return_types(std::vector<DONSUS_TYPE>);

  llvm::Value *compile(utility::handle<donsus_ast::node> &node,
                       utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::variable_decl &ca_ast,
                     utility::handle<DonsusSymTable> &table,
                     bool is_definition = false);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::assignment &ac_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::identifier &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::number_expr &ac_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::function_decl &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::function_def &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::function_call &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::if_statement &ac_ast,
                     utility::handle<donsus_ast::node> &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::else_statement &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::return_kw &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::string_expr &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::float_expr &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::expression &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::print_expr &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::bool_expr &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::unary_expr &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  [[nodiscard]] std::string printf_format(DONSUS_TYPE type) const;

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::array_decl &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::array_def &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::array_access &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Type *map_type(DONSUS_TYPE type);
  llvm::Type *map_pointer_type(DONSUS_TYPE type);

  // meta
  llvm::BasicBlock *main_block;
  llvm::Function *main_func;
  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> TheModule;
  Platform platform;
};

} // namespace DonsusCodegen
#endif