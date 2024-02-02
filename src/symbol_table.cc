// PUBLIC SYMBOL TABLE API

#include "../Include/symbol_table.h"
#include <memory>

std::unique_ptr<donsus_symtable> donsus_sym(const std::string file_name){
    // create symbol table
    std::unique_ptr<donsus_symtable> sym = std::make_unique<donsus_symtable>(file_name);
    return sym;
}

std::unique_ptr<donsus_symtable> donsus_symbol(std::unique_ptr<donsus_symtable> sym, const std::string name){
    // add symbol to symtable
    sym->DONSUS_SYM[name];
    return sym;
}