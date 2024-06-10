#ifndef UTILITY_EXCEPTION_H
#define UTILITY_EXCEPTION_H
#include "terminal_coloured.h"
#include <exception>
#include <iostream>

#include "token.h"

// Header only lib
/**
 *  \brief Represents the base exception class
 */
enum ErrorValueKind: uint32_t {
  ErrorValue_Error,
  ErrorValue_Warning
};

struct ErrorValue {
  ErrorValueKind kind;

};
class DonsusException : public std::exception {
private:
  std::string error_message;

public:
  DonsusException() = default;
  DonsusException(const std::string &message) : error_message(message) {}

  // Returned value shouldn't be ignored
  [[nodiscard]] virtual const char *what() const throw() {
    return error_message.c_str();
  }
};

// Note: defined in exception.cc
class DonsusParserError {
public:
  DonsusParserError() = default;
  void syntax_error_normal(unsigned int column,
                           unsigned int line,
                           const std::string &message,
                           const std::string &full_path);

  void type_error_normal(unsigned int column,
                         unsigned int line,
                         const std::string &message,
                         const std::string &full_path);

  void error_out_coloured(const std::string &message, rang::fg colour = rang::fg::red);

  void error_out_empty();

  void error(const std::string &message);

private:
  bool has_ansi_colours();
};

class ReturnTypeException : public DonsusException {
public:
  ReturnTypeException() = default;
  ReturnTypeException(const std::string &message) : DonsusException(message) {}
};

class InCompatibleTypeException : public DonsusException {
public:
  InCompatibleTypeException() = default;
  InCompatibleTypeException(const std::string &message)
      : DonsusException(message) {}
};

class ReDefinitionException : public DonsusException {
public:
  ReDefinitionException() = default;
  ReDefinitionException(const std::string &message)
      : DonsusException(message) {}
};
class DonsusUndefinedException : public DonsusException {
public:
  DonsusUndefinedException() = default;
  DonsusUndefinedException(const std::string &message)
      : DonsusException(message) {}
};
class UnsignedTypeException : public DonsusException {
public:
  UnsignedTypeException() = default;
  UnsignedTypeException(const std::string &message)
      : DonsusException(message) {}
};
class OutOfBoundException : public DonsusException {
public:
  OutOfBoundException() = default;
  OutOfBoundException(const std::string &message) : DonsusException(message) {}
};
class NotMatchingSubscript : public DonsusException {
public:
  NotMatchingSubscript() = default;
  NotMatchingSubscript(const std::string &message) : DonsusException(message) {}
};
#endif