#include "../Include/symbol_table.h"

DonsusSymTable::DonsusSymTable() : allocator(1024) {}

std::string DonsusSymTable::add(std::string short_name_c, DONSUS_TYPE type) {
  /*
   * add a minimal version of the symbol to the symbol table
   * */
  auto qualified_name = create_qualified_name(short_name_c);
  // mymodule.short_name
  sym t_symbol = {
      .type = type,
      .index = underlying.size(),
      .key = qualified_name,
      .short_name = short_name_c,
      .kind = sym::SYMBOL_PLACEHOLDER,
  };

  underlying.push_back(t_symbol);
  return qualified_name;
}

std::string DonsusSymTable::add(std::string short_name_c,
                                std::vector<DONSUS_TYPE> &types) {
  auto qualified_name = create_qualified_name(short_name);
  sym t_symbol = {
      .types = types,
      .index = underlying.size(),
      .key = qualified_name,
      .short_name = short_name_c,
      .kind = sym::SYMBOL_PLACEHOLDER,
  };
  underlying.push_back(t_symbol);
  return qualified_name;
}

std::string DonsusSymTable::add(std::string short_name_c, DONSUS_TYPE type,
                                bool is_function_argument) {
  auto qualified_name = create_qualified_name(short_name);
  sym t_symbol = {
      .is_function_arg = is_function_argument,
      .type = type,
      .index = underlying.size(),
      .key = qualified_name,
      .short_name = short_name_c,
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
  sym_ptr->short_name = qa_sym_ex;
  sym_ptr->parent = this;
  sym_table.push_back(sym_ptr);
  return sym_ptr;
}

utility::handle<DonsusSymTable>
DonsusSymTable::get_sym_table(std::string &qa_sym_ex) {
  if (qa_sym == qa_sym_ex)
    return this;

  for (auto n : sym_table) {
    if (n->qa_sym == qa_sym_ex)
      return n;
  }

  while (parent) {
    if (parent->qa_sym == qa_sym_ex) {
      return parent;
    } else {
      parent = parent->parent;
    }
  }

  return nullptr;
}

int DonsusSymTable::get_function_argument_size() {
  int size = 0;
  for (auto &n : underlying) {
    if (n.is_function_arg) {
      size++;
    }
  }
  return size;
}

DONSUS_TYPE DonsusSymTable::get_function_argument(int index) {
  return underlying[index].type;
}
bool DonsusSymTable::is_sym_table_exist(std::string &qa_sym_ex) {
  if (get_sym_table(qa_sym_ex)) {
    return true;
  }
  return false;
}

auto DonsusSymTable::get(std::string qualified_name) -> sym {
  sym b;
  b = get_from_qualified_name(qualified_name);
  if (b.mod != -1) {
    return b;
  }

  while (parent) {
    b = parent->get_from_qualified_name(qualified_name);
    if (b.mod == -1 && parent->parent) {
      parent = parent->parent;
    } else {
      return b;
    }
  }
  return b;
}

auto DonsusSymTable::setInst(std::string qualified_name, llvm::Value *inst)
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
  for (auto &n : underlying) {
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

std::string DonsusSymTable::create_qualified_name(std::string &short_name_c) {
  return this->qa_sym + '.' + short_name_c;
}

std::string DonsusSymTable::apply_scope(std::string &name) {
  // propagate up - like recursion
  if (name.empty()) {
    // Handle empty string case
    return "";
  }
  if (name == short_name) {
    // if the symbol name is same as the function
    return qa_sym;
  }
  // Return substring excluding the last character
  std::string result = create_qualified_name(name);
  // this->qa_sym.substr(0, this->qa_sym.size() - 1) + '.' + name;
  return result;
}
