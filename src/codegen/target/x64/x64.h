#ifndef X_64_H
#define X_64_H
#include <iostream>

void donsus_codegen_x64(DonsusParser::parse_result, utility::handle<donsus_symtable> symtable,
                                                        std::string &filename);
#endif