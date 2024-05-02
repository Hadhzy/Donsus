#ifndef DONSUS_PARSER_UTIL
#define DONSUS_PARSER_UTIL
#include "../Include/Internal/type.h"
#include "../src/ast/node.h"

DONSUS_TYPE make_type(donsus_token_kind type);
DONSUS_TYPE make_type_array(donsus_ast::ArrayType type);
#endif