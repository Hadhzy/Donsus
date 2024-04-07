// Symbol table declaration
#ifndef DONSUS_SYMBOL_TABLE_H
#define DONSUS_SYMBOL_TABLE_H
#include "../Include/Internal/type.h"
#include "../src/utility/handle.h"
#include "../src/utility/memory_alloc.h"
#include "llvm/IR/Instructions.h"
#include <iostream>
#include <string>
#include <vector>

#define DEBUG_SYMBOL_TABLE 1

class DonsusSymTable {
public:
  std::vector<utility::handle<DonsusSymTable>> sym_table;
  std::string qa_sym = "global";
  std::vector<DONSUS_TYPE> function_return_type; // only for function def
  struct sym {
    int mod;
    DONSUS_TYPE type;
    std::vector<DONSUS_TYPE> types; // if they are stored as a group
    llvm::AllocaInst *inst;         // for codegen instruction
    bool duplicated = false; // true if its duplicated in the symbol_table
    std::size_t index;       // the order in which the addition happened
    std::string key;         // qualified_name
    std::string short_name;  // the name from which the qualified_name obtained
    enum : int { SORT_WHITE, SORT_GREY, SORT_BLACK };
    enum : int { SYMBOL_GLOBAL, SYMBOL_TYPE, SYMBOL_PLACEHOLDER } kind;
    /*    union {
          d_proc, d_global, d_type, d_imas
        }; // d_proc, d_global, d_type, d_imas*/
    bool operator==(sym const &rhs) const {
      // use when debugging
      return key == rhs.key && type == rhs.type && short_name == rhs.short_name;
    }
    bool operator!=(sym const &rhs) const { return !operator==(rhs); }
  };
  using sym = DonsusSymTable::sym;
  // Debug print for symbol
  friend std::ostream &operator<<(std::ostream &o, sym sma) {
    o << "key: " << sma.key << "\n";
    o << "short_name: " << sma.short_name << "\n";
    o << "index: " << sma.index << "\n";
    return o;
  };
  // Debug print for symbol_table
  friend std::ostream &operator<<(std::ostream &o,
                                  utility::handle<DonsusSymTable> &table) {
    o << "SymbolTable-key: " << table->qa_sym
      << "\n"; // the name of the current symbol table
    o << "SymbolTable-child-count: " << table->sym_table.size()
      << "\n"; // the child symbol tables associated with the symbol table
#ifdef DEBUG_SYMBOL_TABLE
    for (auto &n : table->underlying) {
      o << n;
    }
    o << "\n";
    o << "CHILDREN-TABLES of " << table->qa_sym << "\n";
    for (auto &n : table->sym_table) {
      o << n;
    }
#endif
    return o;
  };
  DonsusSymTable();
  /*
   * Add a minimal symbol representation to the current table
   * */
  std::string add(std::string short_name, DONSUS_TYPE type);

  std::string add(std::string short_name, std::vector<DONSUS_TYPE> &type);
  /*
   * Add symbol table to global.
   * */
  utility::handle<DonsusSymTable> add_sym_table(std::string qa_sym_ex);

  DONSUS_TYPE
  get_function_argument(int index);

  /*
   * Applies the qualifed name to a pass name to compare
   */

  std::string apply_scope(std::string &name);

  /*
   * Obtain symbol table based on name
   * */
  utility::handle<DonsusSymTable> get_sym_table(std::string &qa_sym_ex);

  bool is_sym_table_exist(std::string &qa_sym_ex,
                          utility::handle<DonsusSymTable> current_sym);

  /*
   * Add symbol manually mainly for debugging
   *
   * */
  std::string add_symbol(DonsusSymTable::sym symbol);
  // get symbol based on qualified name
  auto get(std::string qualified_name) -> sym;

  void setInst(std::string qualified_name, llvm::AllocaInst *inst);

  // for debugging purposes
  bool operator==(DonsusSymTable const &rhs) const {
    return underlying == rhs.underlying && sym_table == rhs.sym_table;
  }

private:
  int add_desc(sym &desc);
  // create qualified name from short_name(private)
  std::string create_qualified_name(std::string &short_name);

  // get the symbol based on the qualified name(private)
  auto get_from_qualified_name(std::string &qualified_name) -> sym;
  // holds underlying symbols(private)
  std::vector<sym> underlying;
  utility::DonsusAllocator allocator;
};

#endif