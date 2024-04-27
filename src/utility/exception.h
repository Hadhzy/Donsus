#ifndef UTILITY_EXCEPTION_H
#define UTILITY_EXCEPTION_H
#include <exception>
#include <iostream>

/**
 *  \brief Represents the base exception class
 */
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
#endif