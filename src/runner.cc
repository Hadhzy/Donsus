// Running each step in the compiler
#include "../Include/codegen/codegen.h"
#include "../Include/file.h"
#include "../Include/sema.h"
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

  // build symbol table
  // sema and typecheck
  // see sema.cc for further information
  parser_result->init_traverse();
  parser_result->traverse(donsus_sym, assign_type_to_node, sym_global);

  // codegen
#ifdef DEBUG
  std::cout << "\n";
  std::cout << "SYMBOL TABLE:" << std::endl;
  std::cout << "GLOBAL: " << std::endl;
  std::cout << sym_global << std::endl;
#endif
  // sema

  // codegen
  DonsusCodegen::DonsusCodeGenerator codegen;
  codegen.compile(parser_result);

  /*  delete sym_global.get();*/
  return 0;
}