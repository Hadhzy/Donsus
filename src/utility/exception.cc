#include "exception.h"

void DonsusParserError::syntax_error_normal(const unsigned int &column,
                                            const unsigned int &line,
                                            const std::string &message,
                                            const std::string &full_path) {
  std::string message_c =
      "(" + std::to_string(column) + ":" + std::to_string(line) + ")";

  std::string line_c = message + " AT LINE: " + std::to_string(line);
  if (has_ansi_colours()) {
    error_out_coloured(full_path, rang::fg::reset);
    error_out_coloured(message_c, rang::fg::green);
    error_out_coloured(" ERROR: ");
    error_out_coloured(line_c, rang::fg::reset);
    error_out_coloured("\n");
  } else {
    error(full_path);
    error(" ERROR: ");
    error(line_c);
    error("\n");
  }
}

void DonsusParserError::error_out_coloured(const std::string &message,
                                           rang::fg colour) {
  std::cout << rang::style::bold << colour << message << rang::style::reset
            << rang::fg::reset;
}

void DonsusParserError::error_out_empty() { error(""); }

void DonsusParserError::error(const std::string &message) {
  std::cout << message;
}

// windows | osx support
// Support other systems might need to be done in a completely another file
bool DonsusParserError::has_ansi_colours() {

  // https://force-color.org/
  char const *force_colour = getenv("FORCE_COLOR");
  if (force_colour != nullptr) {
    return true;
  }
  return false;
}