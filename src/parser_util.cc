//===----------------------------------------------------------------------===//
//
//  Extra helper functions mainly for assigning real donsus types from token
//  types.
//
//
//===----------------------------------------------------------------------===//

#include "../Include/parser_util.h"

DONSUS_TYPE make_type(donsus_token_kind type) {
  DONSUS_TYPE a;
  a.type_un = a.from_parse(type);
  return a;
}

DONSUS_TYPE make_type_array(donsus_ast::ArrayType type) {
  DONSUS_TYPE a;

  switch (type) {
  case donsus_ast::FIXED: {
    a.type_un = DONSUS_TYPE::TYPE_FIXED_ARRAY;
  }
  case donsus_ast::STATIC: {
    a.type_un = DONSUS_TYPE::TYPE_STATIC_ARRAY;
  }
  case donsus_ast::DYNAMIC: {
    a.type_un = DONSUS_TYPE::TYPE_DYNAMIC_ARRAY;
  }
  }
  return a;
}