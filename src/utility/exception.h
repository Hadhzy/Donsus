#ifndef UTILITY_EXCEPTION_H
#define UTILITY_EXCEPTION_H
#include <exception>
#include <iostream>

/**
 *  \brief Represents the base expception class
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
#endif