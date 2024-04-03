#ifndef DONSUS_CODEGEN_H
#define DONSUS_CODEGEN_H

#include "../parser.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <llvm/IR/Verifier.h>
#include <memory>

// Select platform
#ifdef _WIN32
#include "../src/codegen/platform/windows_platform.h"
#endif

#ifdef __unix__
#include "../../src/codegen/platform/linux_platform.h"
#endif

/*#ifdef _WIN32
using PlatformClass = Window
#endif*/

#ifdef __unix__
using PlatformClass = LinuxPlatform;
#endif

namespace DonsusCodegen {
// Handle linking
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
  // destination driven code generating system
public:
  DonsusCodeGenerator() = default;
  DonsusCodeGenerator(std::unique_ptr<llvm::LLVMContext> context,
                      std::unique_ptr<llvm::Module> module,
                      std::unique_ptr<llvm::IRBuilder<>> builder);

  // link the object file to get the executable
  void Link() const;
  // create object file
  int create_object_file();
  // create first basic block for entry point
  void create_entry_point();

  void compile_donsus_expr(DonsusParser::end_result &ast);
  llvm::Value *compile(utility::handle<donsus_ast::node> &node,
                       utility::handle<DonsusSymTable> &table);

  /// CreateEntryBlockAlloca - Create an alloca instruction in the entry block
  /// of the function.  This is used for mutable variables etc.
  // visit functions, emit IR
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

  llvm::Value *visit(donsus_ast::if_statement &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::else_statement &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::return_kw &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::string_expr &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(donsus_ast::expression &ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Value *visit(utility::handle<donsus_ast::node> &ast,
                     donsus_ast::print_expr &ca_ast,
                     utility::handle<DonsusSymTable> &table);

  llvm::Type *map_type(DONSUS_TYPE type);

  // data members
  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> TheModule;
  Platform platform;
  // assigning INST's during codegen
  // DonsusSymTable TheSymbolTable;

  // debug
};

} // namespace DonsusCodegen
#endif