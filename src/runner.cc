// Running each step in the compiler
#include <iostream>
#include "../Include/file.h"
#include "../Include/parser.h"
#include "../Include/sema.h"

int Du_Main(int argc, char **argv) {
    std::string result = handle_file(argv); // process file
    // File operations
    const std::string path = argv[1]; // Obtain path
    const std::string base_filename =  path.substr(path.find_last_of("/\\") + 1); // Obtain file name from path
    std::string::size_type const p(base_filename.find_last_of('.'));
    std::string file_without_extension = base_filename.substr(0, p); // Obtain file without the extension(.du)
    donsus_ast base; // create ast structure obj on the stack

    // Lexer
    donsus_lexer    lexer(result); // initialise lexer
    donsus_parser parser;

    parser.error = false;

    parser.lexer = lexer;
    parser.cur_token = donsus_lexer_next(parser);

    // Parser
    std::unique_ptr<donsus_ast> parser_result = donsus_parse(parser, base);

    // Semantic analysis (Construct symbol table)
    // donsus_ast& sema_result = donsus_sema(parser_result);

    // CODE GENERATION(INVOKE DUASM)
    return 0;
}
