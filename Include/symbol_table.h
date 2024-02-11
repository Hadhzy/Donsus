// Donsus symbol table
#ifndef DONSUS_SYMBOL_TABLE_H
#define DONSUS_SYMBOL_TABLE_H
#include "../Include/donsus.h"
#include "../Include/parser.h"
#include <iostream>
#include <unordered_map>
// Types here

class donsus_symbol {
public:
  donsus_symbol(std::string &name, donsus_token_kind type);

  [[nodiscard]] std::string get_name();

  donsus_token_kind get_type();

  friend std::ostream &operator<<(std::ostream &o, donsus_symbol &symbol);

#ifdef DEBUG
  void debug_symbol() { std::cout << *this; }
#endif

private:
  // name
  std::string name; // might be redundant
  // scope
  // type
  donsus_token_kind type;
  // namespace
  // flags
};

class donsus_symtable {
public:
  donsus_symtable(std::string &file_name);
  std::string &get_file_name();

  std::unordered_map<std::string, donsus_symbol> get_sym();

  friend std::ostream &operator<<(std::ostream &o, donsus_symtable &symtable);

#ifdef DEBUG
  void debug_symtable() { std::cout << *this; }
#endif
private:
  std::string file_name; // file name
  // symbol table type

  std::unordered_map<std::string, donsus_symbol> DONSUS_SYM; // hash table
};

std::unique_ptr<donsus_symtable> donsus_sym_make(std::string &file_name);
std::unique_ptr<donsus_symbol> donsus_symbol_make(std::string &name,
                                                  donsus_token_kind type);

#endif