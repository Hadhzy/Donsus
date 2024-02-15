// PUBLIC SYMBOL TABLE API
#include "../Include/symbol_table.h"
#include "../src/utility/memory_alloc.h"
#include <memory>

// donsus_symbol
donsus_symbol::donsus_symbol(std::string &name, donsus_token_kind type)
    : name(name), type(type){};

std::string donsus_symbol::get_name() { return name; }

donsus_token_kind donsus_symbol::get_type() { return type; }

// donsus_symtable
donsus_symtable::donsus_symtable(std::string &file_name)
    : file_name(file_name){};

std::string &donsus_symtable::get_file_name() { return file_name; }

std::unordered_map<std::string, donsus_symbol> donsus_symtable::get_sym() {
  return DONSUS_SYM;
}

std::ostream &operator<<(std::ostream &o, donsus_symbol &symbol) {
  // prints a symbol
  o << "Name:  " << symbol.get_name() << "\n";
  return o;
}

std::ostream &operator<<(std::ostream &o, donsus_symtable &symtable) {
  auto print_sym = [](auto &key, auto &value, std::ostream &o) {
    o << "Key:[" << key << "] Value:[" << value << "]\n";
  };

  for (auto &n : symtable.DONSUS_SYM)
    print_sym(n.first, n.second, o);
  return o;
}

// PUBLIC API
utility::handle<donsus_symtable> donsus_sym_make(std::string &file_name, utility::DonsusAllocator alloc) {
  // create symbol table
  // redesign this part
  utility::handle sym = alloc.alloc_constructor<donsus_symtable>(file_name);
  return sym;
}

/*
utility::handle<donsus_symbol> donsus_symbol_make(std::string &name,
                                                  donsus_token_kind type) {
  // create symbol
  // redesign this part
  std::unique_ptr<donsus_symbol> symbol =
      std::make_unique<donsus_symbol>(name, type);
  return symbol;
}
*/
