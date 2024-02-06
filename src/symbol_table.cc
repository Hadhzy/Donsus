// PUBLIC SYMBOL TABLE API
#include "../Include/symbol_table.h"
#include <memory>

// donsus_symbol
donsus_symbol::donsus_symbol(std::string& name, donsus_token_kind type): name(name), type(type) {};

std::string donsus_symbol::get_name() const{
    return name;
}

donsus_token_kind donsus_symbol::get_type(){
    return type;
}

// donsus_symtable
donsus_symtable::donsus_symtable(std::string& file_name): file_name(file_name) {};

const std::string& donsus_symtable::get_file_name() {
    return file_name;
}
std::unordered_map<std::string, donsus_symbol> donsus_symtable::get_sym() {
    return DONSUS_SYM;
}

std::ostream& operator<<(std::ostream &o, const donsus_symbol& symbol){
    // prints a symbol
    o << "Name:  " << symbol.get_name() << "\n";
    return o;
}

std::ostream& operator<<(std::ostream &o, donsus_symtable& symtable){
    auto print_sym = [](const auto& key, const auto& value, std::ostream &o){
        o << "Key:[" << key << "] Value:[" << value << "]\n";
    };

    for (const auto& n: symtable.DONSUS_SYM)
        print_sym(n.first, n.second, o);
    return o;
}

// PUBLIC API
std::unique_ptr<donsus_symtable> donsus_sym_make(std::string& file_name){
    // create symbol table
    std::unique_ptr<donsus_symtable> sym = std::make_unique<donsus_symtable>(file_name);
    return sym;
}

std::unique_ptr<donsus_symbol> donsus_symbol_make(std::string& name, donsus_token_kind type){
    // create symbol
    std::unique_ptr<donsus_symbol> symbol = std::make_unique<donsus_symbol>(name, type);
    return symbol;
}