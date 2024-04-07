#include "../Include/symbol_table.h"

DonsusSymTable::DonsusSymTable() : allocator(1024) {}

std::string DonsusSymTable::add(std::string short_name, DONSUS_TYPE type) {
  /*
   * add a minimal version of the symbol to the symbol table
   * */
  auto qualified_name = create_qualified_name(short_name);
  // mymodule.short_name
  sym t_symbol = {
      .type = type,
      .index = underlying.size(),
      .key = qualified_name,
      .short_name = short_name,
      .kind = sym::SYMBOL_PLACEHOLDER,
  };

  underlying.push_back(t_symbol);
  return qualified_name;
}

std::string DonsusSymTable::add(std::string short_name,
                                std::vector<DONSUS_TYPE> &types) {
  auto qualified_name = create_qualified_name(short_name);
  sym t_symbol = {
      .types = types,
      .index = underlying.size(),
      .key = qualified_name,
      .short_name = short_name,
      .kind = sym::SYMBOL_PLACEHOLDER,
  };
  underlying.push_back(t_symbol);
  return qualified_name;
}

std::string DonsusSymTable::add_symbol(DonsusSymTable::sym symbol) {
  underlying.push_back(symbol);
  auto qualified_name = create_qualified_name(symbol.short_name);
  return qualified_name;
}

utility::handle<DonsusSymTable>
DonsusSymTable::add_sym_table(std::string qa_sym_ex) {
  const utility::handle sym_ptr = allocator.r_alloc<DonsusSymTable>();
  sym_ptr->qa_sym = create_qualified_name(qa_sym_ex);
  sym_table.push_back(sym_ptr);
  return sym_ptr;
}

utility::handle<DonsusSymTable>
DonsusSymTable::get_sym_table(std::string &qa_sym_ex) {
  for (auto n : sym_table) {
    if (n->qa_sym == qa_sym_ex)
      return n;
  }
}

DONSUS_TYPE DonsusSymTable::get_function_argument(int index) {
  return underlying[index].type;
}
bool DonsusSymTable::is_sym_table_exist(
    std::string &qa_sym_ex, utility::handle<DonsusSymTable> current_sym) {
  int found = 0;
  for (auto n : current_sym->sym_table) {
    if (n->qa_sym == qa_sym_ex)
      found++;
  }
  if (found == 1)
    return true;
  return false;
}

auto DonsusSymTable::get(std::string qualified_name) -> sym {
  sym b = get_from_qualified_name(qualified_name);
  if (b.mod == -1) {
    // doesn't exist
    return b; // empty
  }
  return b;
}

auto DonsusSymTable::setInst(std::string qualified_name, llvm::AllocaInst *inst)
    -> void {
  for (auto &n : underlying) {
    if (n.key == create_qualified_name(qualified_name)) {
      n.inst = inst;
    }
  }
}

int DonsusSymTable::add_desc(sym &desc) {
  if (desc.mod != -1 && desc.kind != sym::SYMBOL_PLACEHOLDER) {
    std::cout << "report error here";
    return 1;
  }
  underlying.push_back(desc);
  return 0;
}

auto DonsusSymTable::get_from_qualified_name(std::string &qualified_name)
    -> sym {
  sym a;
  int found = 0;
  for (auto n : underlying) {
    if (n.key == create_qualified_name(qualified_name)) {
      a = n;
      ++found;
    }
  }
  if (found >= 2) {
    a.duplicated = true;
    return a;
  } else if (found == 1) {
    return a;
  }

  sym n{.mod = -1};
  return n;
}

std::string DonsusSymTable::create_qualified_name(std::string &short_name) {
  return this->qa_sym + '.' + short_name;
}

std::string DonsusSymTable::apply_scope(std::string &name) {
  if (name.empty()) {
    // Handle empty string case
    return "";
  }
  // Return substring excluding the last character
  std::string result = create_qualified_name(name);
  // this->qa_sym.substr(0, this->qa_sym.size() - 1) + '.' + name;
  return result;
}
