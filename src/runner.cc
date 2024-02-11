// Running each step in the compiler
#include "../Include/file.h"
#include "../Include/parser.h"
#include "../Include/sema.h"
#include "../Include/symbol_table.h"
#include "./codegen/target/x64/x64.h"
#include <iostream>
#include <memory>

int Du_Main(int argc, char **argv) {

  std::string result = handle_file(argv); // process file
  // File operations
  std::string path = argv[1]; // Obtain path
  const std::string base_filename =
      path.substr(path.find_last_of("/\\") + 1); // Obtain file name from path
  std::string::size_type const p(base_filename.find_last_of('.'));

  std::string file_without_extension =
      base_filename.substr(0, p); // Obtain file without the extension(.du)
  donsus_ast base;                // create ast structure obj on the stack

  // Lexer
  donsus_lexer lexer(result); // initialise lexer
  DonsusParser parser(lexer);

  // Parser
  std::unique_ptr<donsus_global_ast> parser_result = parser.donsus_parse();

  // Semantic analysis (Construct symbol table)
  std::unique_ptr<donsus_symtable> symtable_result = donsus_sym(path);

  // nothing currently Todo: catch value and everything(sema_result)
  std::unique_ptr<donsus_global_ast> sema_result =
      donsus_sema(std::move(parser_result), file_without_extension);

  // CODE GENERATION(INVOKE DUASM) Todo: use sema_result
  donsus_codegen_x64(std::move(sema_result), std::move(symtable_result),
                     file_without_extension);

  return 0;
}
