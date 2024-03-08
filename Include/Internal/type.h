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

  /**
   *  \brief  Convert token kind(from lexer)
   * to actual Donsus Type
   */
  kind from_parse(donsus_token_kind type) {
    switch (type) {
    case DONSUS_BASIC_INT:
      return TYPE_BASIC_INT;

    case DONSUS_I32:
      return TYPE_I32;

    case DONSUS_U64:
      return TYPE_U64;

    case DONSUS_CHAR:
      return TYPE_CHAR;

    case DONSUS_I8:
      return TYPE_I8;

    case DONSUS_I64:
      return TYPE_I64;

    case DONSUS_BOOL:
      return TYPE_BOOL;

    case DONSUS_I16:
      return TYPE_I16;

    case DONSUS_U32:
      return TYPE_U32;

    case DONSUS_VOID:
      return TYPE_VOID;

    default: {
      // Handle unknown token kind or other cases
      std::cerr << "Unhandled token kind in DONSUS_TYPE::from_parse"
                << std::endl;
      return TYPE_UNKNOWN;
    }
    }
  }

  donsus_token_kind to_parse(kind type) {
    switch (type) {
    case TYPE_BASIC_INT:
      return DONSUS_BASIC_INT;

    case TYPE_I32:
      return DONSUS_I32;

    case TYPE_U64:
      return DONSUS_U64;

    case TYPE_CHAR:
      return DONSUS_CHAR;

    case TYPE_I8:
      return DONSUS_I8;

    case TYPE_I64:
      return DONSUS_I64;

    case TYPE_BOOL:
      return DONSUS_BOOL;

    case TYPE_I16:
      return DONSUS_I16;

    case TYPE_U32:
      return DONSUS_U32;

    case TYPE_VOID:
      return DONSUS_VOID;

    default: {
      // Handle unknown token kind or other cases
      std::cerr << "Unhandled type in to_parse" << std::endl;
      return DONSUS_END;
    }
    }
  }

  kind type_un;
};

/**
 *  \brief
 *  identifier:type
 */
struct NAME_DATA_PAIR {
  DONSUS_TYPE type;
  std::string identifier;
};
#endif