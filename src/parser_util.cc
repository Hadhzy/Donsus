#include "../Include/parser_util.h"

DONSUS_TYPE make_type(donsus_token_kind type) {
  DONSUS_TYPE a;
  a.type_un = a.from_parse(type);
  return a;
}