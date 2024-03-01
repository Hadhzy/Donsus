// Running each step in the compiler
#include "../Include/donsus.h"
#include "../Include/file.h"
#include "../Include/parser.h"
#include "../Include/sema.h"
#include <iostream>

int Du_Main(int argc, char **argv) {

  std::string result = handle_file(argv);
  std::string path = argv[1]; // Obtain path
  std::string base_filename =
      path.substr(path.find_last_of("/\\") + 1); // Obtain file name from path

  std::string::size_type p(base_filename.find_last_of('.'));

  std::string file_without_extension =
      base_filename.substr(0, p); // Obtain file without the extension(.du)

  // Lexer
  donsus_lexer lexer(result); // initialise lexer
  DonsusParser parser(lexer);

  // Parser
  DonsusParser::end_result parser_result = parser.donsus_parse();

  utility::handle<DonsusSymTable> sym_global = new DonsusSymTable();

  // build symbol table
  parser_result->traverse(donsus_sym, sym_global);

#ifdef DEBUG
  std::cout << "\n";
  std::cout << "SYMBOL TABLE:" << std::endl;
  std::cout << "GLOBAL: " << std::endl;
  std::cout << sym_global << std::endl;
#endif
  // sema
  DonsusSema sema;

  DonsusSema::end_result sema_result =
      sema.donsus_sema(parser_result, file_without_extension);

  /*   donsus_codegen_x64(sema_result, symtable_result,
   * file_without_extension);*/

  return 0;
}
