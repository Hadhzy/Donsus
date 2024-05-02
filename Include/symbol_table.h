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
  std::string short_name = "global";
  std::vector<DONSUS_TYPE> function_return_type; // only for function def
  llvm::Value *inst;
  // similar to inst - requires a loop to change the actual value
  // e.g setInst
  struct f_mul_ret {
    llvm::Type *type_of_struct;
    llvm::Value *inst_of_struct;
  } multiple_return;
  utility::handle<DonsusSymTable> parent = nullptr; //
  struct sym {
    int mod;

    struct donsus_array {
      // used for looping
      unsigned int num_of_elems;
      // for example - we can load this in printf
      std::vector<llvm::Value *> insts;
      DONSUS_TYPE type; // all elements must have same type
    };

    // set if symbol holds an array
    donsus_array array;

    bool is_function_arg = false;
    DONSUS_TYPE type;
    std::vector<DONSUS_TYPE> types; // if they are stored as a group
    llvm::Value
        *inst; // This can represent AllocaDef, or a function as an lvalue
    bool duplicated = false; // true if its duplicated in the symbol_table
    std::size_t index;       // the order in which the addition happened
    std::string key;         // qualified_name
    std::string short_name;  // the name from which the qualified_name obtained
    enum : int { SYMBOL_GLOBAL, SYMBOL_TYPE, SYMBOL_PLACEHOLDER } kind;
    bool operator==(sym const &rhs) const {
      // use when debugging
      return key == rhs.key && type == rhs.type && short_name == rhs.short_name;
    }
    bool operator!=(sym const &rhs) const { return !operator==(rhs); }
  };
  using sym = DonsusSymTable::sym;
  // Debug print for symbol
  friend std::ostream &operator<<(std::ostream &o, sym &sma) {
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

  std::string add(std::string short_name, DONSUS_TYPE type,
                  bool is_function_argument);
  /*
   * Add symbol table to global.
   * */
  utility::handle<DonsusSymTable> add_sym_table(std::string qa_sym_ex);

  DONSUS_TYPE
  get_function_argument(int index);

  int get_function_argument_size();

  /*
   * Applies the qualified name to a pass name to compare
   */

  std::string apply_scope(std::string &name);

  /*
   * Obtain symbol table based on name
   * */
  utility::handle<DonsusSymTable> get_sym_table(std::string &qa_sym_ex);

  utility::handle<DonsusSymTable>
  get_sym_table_from_unqualified(std::string &qa_sym_ex);
  // Check if table exists inside a symbol table
  // it also checks itself as one of the options
  bool is_sym_table_exist(std::string &qa_sym_ex);

  /*
   * Add symbol manually mainly for debugging
   *
   * */
  std::string add_symbol(DonsusSymTable::sym symbol);
  // get symbol based on qualified name
  auto get(std::string qualified_name) -> sym;

  // set inst field from CreateAlloca or a function
  void setInst(std::string qualified_name, llvm::Value *inst);

  void setSym(std::string qualified_name, DonsusSymTable::sym &symbol);
  // for debugging purposes
  bool operator==(DonsusSymTable const &rhs) const {
    return underlying == rhs.underlying && sym_table == rhs.sym_table;
  }

private:
  int add_desc(sym &desc);
  // create qualified name from short_name(private)
  std::string create_qualified_name(std::string &short_name);

  // get the symbol based on the qualified name(private), this will propagate
  // to other symbol tables if the current one does not contain the one we need.
  auto get_from_qualified_name(std::string &qualified_name) -> sym;
  // holds underlying symbols(private)
  std::vector<sym> underlying;
  utility::DonsusAllocator allocator;
};

#endif