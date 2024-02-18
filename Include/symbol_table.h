// Symbol table declaration
#ifndef DONSUS_SYMBOL_TABLE_H
#define DONSUS_SYMBOL_TABLE_H
#include <iostream>
#include <string>
#include <vector>

class DonsusSymTable {
public:
  std::vector<DonsusSymTable> sym_table;
  std::string qa_sym = "global";
  struct sym {
    int mod;
    std::size_t index;      // the order in which the addition happened
    std::string key;        // qualified_name
    std::string short_name; // the name from which the qualified_name obtained
    enum : int { SORT_WHITE, SORT_GREY, SORT_BLACK };
    enum : int { SYMBOL_GLOBAL, SYMBOL_TYPE, SYMBOL_PLACEHOLDER } kind;
/*    union {
      d_proc, d_global, d_type, d_imas
    }; // d_proc, d_global, d_type, d_imas*/
  };
  using sym = DonsusSymTable::sym;
  // Debug print for symbol
  friend std::ostream &operator<<(std::ostream &o, sym &sma){
    o << "key: " << sma.key << "\n";
    o << "short_name: " << sma.short_name << "\n";
    o << "index: " << sma.index << "\n";
    return o;
  };
  // Debug print for symbol_table
  friend std::ostream &operator<<(std::ostream &o, DonsusSymTable &table){
    o << "SymbolTable-key: " << table.qa_sym << "\n";
    o << "SymbolTable-child-count: " << table.sym_table.size();
    return o;
  };

  DonsusSymTable() = default;
  /*
   * Add a minimal symbol representation to the current table
   * */
  std::string add(std::string& short_name);

  /*
   * Add symbol table to global.
   * */
  DonsusSymTable add_sym_table(std::string& qa_sym_ex);

  /*
   * Obtain symbol table based on name
   * */
  DonsusSymTable get_sym_table(std::string& qa_sym_ex);

  // get symbol based on qualified name
  auto get(std::string& qualified_name) -> sym;

private:
  int add_desc(sym& desc);
  // create qualified name from short_name(private)
  std::string create_qualified_name(std::string& short_name);

  // get the symbol based on the qualified name(private)
  auto get_from_qualified_name(std::string& qualified_name) -> sym;
  // holds underlying symbols(private)
  std::vector<sym> underlying;
};

#endif