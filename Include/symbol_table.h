// Donsus symbol table
#ifndef DONSUS_SYMBOL_TABLE_H
#define DONSUS_SYMBOL_TABLE_H
#include <iostream>
#include <unordered_map>
#include "../Include/donsus.h"

// Types here

class donsus_symbol{
public:
    donsus_symbol(const std::string name): name(name) {};

     const std::string get_name() const {
        return name;
    }

    friend std::ostream& operator<<(std::ostream &o, donsus_symbol& symbol);

    #ifdef DEBUG
    void debug_symbol(){
         std::cout << *this;
     }
    #endif

private:
    const std::string name;
    // scope
    // type
    // namespace
    // flags
};

class donsus_symtable{
public:
    donsus_symtable(const std::string file_name): file_name(file_name) {};
    const std::string get_file_name(){
        return file_name;
    }
    std::unordered_map<std::string, donsus_symbol> get_sym(){
        return DONSUS_SYM;
    }

    friend std::ostream& operator<<(std::ostream &o, donsus_symtable& symtable);

    #ifdef DEBUG
    void debug_symtable(){
        std::cout << *this;
    }
    #endif
private:
    const std::string file_name; // file name
    // symbol table type

    std::unordered_map<std::string, donsus_symbol> DONSUS_SYM; // hash table
};

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
#endif