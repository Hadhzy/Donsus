#include "../Include/Internal/type.h"
//===----------------------------------------------------------------------===//
//
//  This file performs different kinds of transformations from tokens to donsus
//  types and vice-versa. Not every single type can be converted back to a token
//  kind. Generally, this scenario only applies to primitive types.
//===----------------------------------------------------------------------===//

/**
 *  \brief  Convert token kind(from lexer)
 * to actual Donsus Type
 */
DONSUS_TYPE::kind DONSUS_TYPE::from_parse(donsus_token_kind type) {
  switch (type) {
  case DONSUS_BASIC_INT:
    return TYPE_BASIC_INT;

  case DONSUS_I32:
    return TYPE_I32;

  case DONSUS_U64:
    return TYPE_U64;

  case DONSUS_STRING_TYPE:
    return TYPE_STRING;

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

  case DONSUS_F32:
    return TYPE_F32;

  case DONSUS_F64:
    return TYPE_F64;

  case DONSUS_VOID:
    return TYPE_VOID;

  default: {
    // Handle unknown token kind or other cases
    /*      std::cerr << "Unhandled token kind in DONSUS_TYPE::from_parse"
                    << std::endl;*/
    return TYPE_UNKNOWN;
  }
  }
}

/**
 *  \brief Convert Donsus Type to token kind
 */
donsus_token_kind DONSUS_TYPE::to_parse(DONSUS_TYPE::kind type) {
  switch (type) {
  case TYPE_BASIC_INT:
    return DONSUS_BASIC_INT;

  case TYPE_I32:
    return DONSUS_I32;

  case TYPE_U64:
    return DONSUS_U64;

  case TYPE_STRING:
    return DONSUS_STRING;

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

  case TYPE_F32:
    return DONSUS_F32;

  case TYPE_F64:
    return DONSUS_F64;

  case TYPE_VOID:
    return DONSUS_VOID;

  default: {
    // Handle unknown token kind or other cases
    std::cerr << "Unhandled type in to_parse" << std::endl;
    return DONSUS_END;
  }
  }
}

auto DONSUS_TYPE::to_string() const -> std::string {
  switch (type_un) {
  case TYPE_BASIC_INT:
    return "TYPE_BASIC_INT";
  case TYPE_I32:
    return "TYPE_I32";
  case TYPE_U64:
    return "TYPE_U64";
  case TYPE_STRING:
    return "TYPE_STRING";
  case TYPE_I8:
    return "TYPE_I8";
  case TYPE_I64:
    return "TYPE_I64";
  case TYPE_BOOL:
    return "TYPE_BOOL";
  case TYPE_I16:
    return "TYPE_I16";
  case TYPE_U32:
    return "TYPE_U32";
  case TYPE_F32:
    return "TYPE_F32";
  case TYPE_F64:
    return "TYPE_F64";
  case TYPE_VOID:
    return "TYPE_VOID";
  case TYPE_FIXED_ARRAY:
    return "TYPE_FIXED_ARRAY";
  case TYPE_DYNAMIC_ARRAY:
    return "TYPE_DYNAMIC_ARRAY";
  case TYPE_STATIC_ARRAY:
    return "TYPE_STATIC_ARRAY";
  default: {
  }
  }
}

auto DONSUS_TYPE::is_integer() const -> bool {
  if (this->type_un == TYPE_BASIC_INT || this->type_un == TYPE_I32 ||
      this->type_un == TYPE_U64 || this->type_un == TYPE_I8 ||
      this->type_un == TYPE_I64 || this->type_un == TYPE_I16 ||
      this->type_un == TYPE_U32) {
    return true;
  } else
    return false;
}