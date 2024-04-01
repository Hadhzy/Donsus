// Running each step in the compiler
#include "../Include/codegen/codegen.h"
#include "../Include/file.h"
#include "../Include/sema.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
// https://stackoverflow.com/questions/56894943/using-passmanager-in-llvm-6

#include <iostream>

DonsusParser::end_result Du_Parse(std::string result) {
  // Lexer
  donsus_lexer lexer(result); // initialise lexer
  DonsusParser parser(lexer);
  // Parser
  DonsusParser::end_result parser_result = parser.donsus_parse();
  return parser_result;
}

int Du_Main(int argc, char **argv) {
  std::string result = handle_file(argv[1]);
  std::string path = argv[1]; // Obtain path
  std::string base_filename =
      path.substr(path.find_last_of("/\\") + 1); // Obtain file name from path

  std::string::size_type p(base_filename.find_last_of('.'));

  std::string file_without_extension =
      base_filename.substr(0, p); // Obtain file without the extension(.du)

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  DonsusParser::end_result parser_result = Du_Parse(result);

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

  // Todo: This section can be implemented inside codegen.cc as 2 methods
  // Initialize the target registry etc.
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  codegen.TheModule->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
  if (!Target) {
    llvm::errs() << Error;
    return 1;
  }

  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions opt;
  auto TheTargetMachine = Target->createTargetMachine(
      TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

  codegen.TheModule->setDataLayout(TheTargetMachine->createDataLayout());

  auto Filename = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }

  llvm::legacy::PassManager pass;
  const auto FileType = llvm::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*codegen.TheModule);
  dest.flush();

  llvm::outs() << "Wrote " << Filename << "\n";

  // codegen
#ifdef DEBUG
  std::cout << "\n";
  std::cout << "SYMBOL TABLE:" << std::endl;
  std::cout << "GLOBAL: " << std::endl;
  std::cout << sym_global << std::endl;
#endif

  /*  delete sym_global.get();*/
  return 0;
}