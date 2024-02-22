#include "../Include/symbol_table.h"

std::string DonsusSymTable::add(std::string short_name) {
  /*
   * add a minimal version of the symbol to the symbol table
   * */
  auto qualified_name = create_qualified_name(short_name);

  sym a = get_from_qualified_name(qualified_name);
  if (a.mod != -1) {
    // already exists
    std::cout << "symbol already exists: " << a;
  }

  // mymodule.short_name
  sym t_symbol = {
      .index = underlying.size(),
      .key = qualified_name,
      .short_name = short_name,
      .kind = sym::SYMBOL_PLACEHOLDER,
  };

  underlying.push_back(t_symbol);
  return qualified_name;
}

utility::handle<DonsusSymTable>
DonsusSymTable::add_sym_table(std::string qa_sym_ex) {
  const utility::handle sym_ptr = allocator.alloc<DonsusSymTable>();
  sym_ptr->qa_sym = qa_sym_ex;
  sym_table.push_back(sym_ptr);
  return sym_ptr;
}

utility::handle<DonsusSymTable>
DonsusSymTable::get_sym_table(std::string &qa_sym_ex) {
  for (auto n : sym_table) {
    if (n->qa_sym == qa_sym_ex)
      return n;
  }
  std::cout << "error";
}

auto DonsusSymTable::get(std::string &qualified_name) -> sym {
  sym b = get_from_qualified_name(qualified_name);
  if (b.mod == -1) {
    // doesn't exist
    std::cout << "doesn't exist";
    return b; // empty
  }
  return b;
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
  for (auto n : underlying) {
    if (n.key == qualified_name)
      return n;
  }
  sym n{.mod = -1};
  return n;
}

std::string DonsusSymTable::create_qualified_name(std::string &short_name) {
  return this->qa_sym + '.' + short_name;
}
