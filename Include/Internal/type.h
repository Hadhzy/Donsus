#ifndef DONSUS_TYPE_H
#define DONSUS_TYPE_H
#include "../token.h"
#include <iostream>

class DONSUS_TYPE {
public:
  enum kind : int {
    TYPE_UNKNOWN = 0, // handled in type checker
    TYPE_BASIC_INT,
    TYPE_I32,
    TYPE_U64,
    TYPE_CHAR,
    TYPE_I8,
    TYPE_I64,
    TYPE_BOOL,
    TYPE_I16,
    TYPE_U32,
    TYPE_VOID
  };

  kind from_parse(donsus_token_kind type);

  donsus_token_kind to_parse(kind type);

  kind type_un;

  bool operator==(const DONSUS_TYPE &rhs) const {
    // TYPE_BASIC_INT and any other integer types are compatible
    if (rhs.type_un == DONSUS_TYPE::kind::TYPE_BASIC_INT && is_integer() ||
        this->type_un == DONSUS_TYPE::kind::TYPE_BASIC_INT &&
            rhs.is_integer()) {
      return true;
    }

    if (this->type_un == rhs.type_un)
      return true;

    return false;
  }
  auto to_string() const -> std::string;
  auto is_integer() const -> bool;
};

/**
 *  \brief
 *  identifier:type in function prototype when defining it
 */
struct NAME_DATA_PAIR {
  DONSUS_TYPE type;
  std::string identifier;
};

/**
 *  \brief
 *  identifier:type or just simply identifier in function calls when calling it
 */
struct NAME_OR_DATA_PAIR {
  std::string identifier;
};
#endif