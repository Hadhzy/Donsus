//===----------------------------------------------------------------------===//
// CLI for the Donsus Compiler
//===----------------------------------------------------------------------===//
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>

#pragma once

namespace DonsusCLI {

class BaseCommand;

constexpr inline int ERROR_RESULT = std::numeric_limits<int>::max();
class BaseCommandGroup {
  std::vector<BaseCommand> commands;

public:
  void add(BaseCommand &com);
  bool get(const std::string &name) const;
  void print() const;
};

class BaseCommand {
  const char *Data;
  BaseCommandGroup Group;

public:
  [[nodiscard]] const char *getData() const;
  bool operator==(const BaseCommand &rhs) const;

  BaseCommand(const char *Data, BaseCommandGroup &Group)
      : Data(Data), Group(Group) {
    Group.add(*this);
  }

  BaseCommand(std::nullptr_t) = delete;
  [[nodiscard]] const char *begin();
  [[nodiscard]] const char *end();
  void print() const;
};

class Parser {
  BaseCommandGroup InitComGroup{};
  char const**src;

public:
  Parser(char const**argv) : src(argv) {
    assert(argv);
    make_chunks();
  }
  void parse(char const*);
  void make_chunks();
  BaseCommandGroup &get_comm_group();
};

void BaseCommandGroup::add(BaseCommand &comm) { commands.push_back(comm); }
bool BaseCommandGroup::get(const std::string &name) const {
  for (auto comm : commands) {
    if (comm.getData() == name) {
      return true;
    }
  }
  return false;
}
void BaseCommandGroup::print() const {
  for (auto comm : commands) {
    comm.print();
  }
}

[[nodiscard]] const char *BaseCommand::getData() const { return Data; }
bool BaseCommand::operator==(const BaseCommand &rhs) const {
  if (this->Data == rhs.getData()) {
    return true;
  } else {
    return false;
  }
}
[[nodiscard]] const char *BaseCommand::begin() { return this->Data; }

[[nodiscard]] const char *BaseCommand::end() {
  return this->Data + std::strlen(this->Data);
}
void BaseCommand::print() const { std::cout << "command: -" << this->Data; }

void Parser::parse(char const*text) {
  if (!text)
    return;
  if (text[0] == '-') {
    char const*restOfText = text + 1;
    BaseCommand comm = BaseCommand(restOfText, InitComGroup);
  }
}
void Parser::make_chunks() {
  for (char const*c = *src; c; c = *++src) {
    parse(c);
  }
}

BaseCommandGroup &Parser::get_comm_group() { return InitComGroup; }
} // namespace DonsusCLI