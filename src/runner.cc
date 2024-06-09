//===----------------------------------------------------------------------===//
// Main entry point for the Donsus Compiler.
// It calls all the steps needed from the top to the bottom.
//===----------------------------------------------------------------------===//

#include "../Include/cli.h"
#include "../Include/codegen/codegen.h"
#include "../Include/file.h"
#include "../Include/sema.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/TargetParser/Host.h"
// https://stackoverflow.com/questions/56894943/using-passmanager-in-llvm-6

#include <iostream>
#include <filesystem>

DonsusParser Du_Parse(std::string result, DonsusAstFile &file) {
  // Lexer
  donsus_lexer lexer(std::move(result)); // initialise lexer
  DonsusParser parser(lexer, file);
  // Parser
  return parser;
}

int Du_Main(int argc, char **argv) {
  DonsusAstFile file;
  DonsusCLI::Parser cli_parser = DonsusCLI::Parser(argv);

  std::string result = handle_file(argv[1]);
  std::string path = argv[1]; // Obtain path
  std::string base_filename =
      path.substr(path.find_last_of("/\\") + 1); // Obtain file name from path

  // check for extension
  std::string::size_type p(base_filename.find_last_of('.'));

  std::string file_without_extension =
      base_filename.substr(0, p); // Obtain file without the extension(.du)

  std::string file_extension = base_filename.substr(p + 1);
  if (file_extension != "du") {
    throw std::runtime_error("File extension must be: .du");
  }

  file.extension = file_extension;
  file.filename = file_without_extension;
  file.fullpath = path;
  file.absolute_path = std::filesystem::absolute(path).string();

  file.id = 0;

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusParser parser = Du_Parse(result, file);
  DonsusParser::end_result parser_result = parser.donsus_parse();

  if (file.error_count) {
    std::cout << rang::fg::reset;
    return 1;
  }
#if DEBUG
  std::cout << "\n";

  std::cout << "-----Parsing completed successfully-----\n";

  std::cout << "\n";
#endif
  // codegen
  std::unique_ptr<llvm::LLVMContext> TheContext =
      std::make_unique<llvm::LLVMContext>();
  std::unique_ptr<llvm::Module> TheModule =
      std::make_unique<llvm::Module>("Donsus IR", *TheContext);
  std::unique_ptr<llvm::IRBuilder<>> TheBuilder =
      std::make_unique<llvm::IRBuilder<>>(*TheContext);

  DonsusCodegen::DonsusCodeGenerator codegen(
      std::move(TheContext), std::move(TheModule), std::move(TheBuilder));

  // sema
  parser_result->init_traverse();
  parser_result->traverse(donsus_sym, assign_type_to_node, sym_global, codegen);

  // Initialise the target registry etc.

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  if (codegen.Builder) {
    /*    codegen.Builder->SetInsertPoint(codegen.main_block);*/
    codegen.Finish();
    codegen.create_object_file();
    codegen.Link();
  }

  // codegen
#if DEBUG
  std::cout << "\n";
  std::cout << "SYMBOL TABLE:" << std::endl;
  std::cout << "GLOBAL: " << std::endl;
  std::cout << sym_global << std::endl;
#endif

// print out llvm IR
#if DEBUG
  std::cout << "-------------------------------------"
            << "\n";
  llvm::errs() << *codegen.TheModule;
#endif
  /*  delete sym_global.get();*/
  // platform support here

  if (cli_parser.get_comm_group().get("run")) {
    std::system("./a.out");
  }

  return 0;
}