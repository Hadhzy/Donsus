#ifndef DONSUS_TYPE_H
#define DONSUS_TYPE_H
#include <iostream>
#include "../token.h"

class DONSUS_TYPE{
public:
  enum kind: int{
    TYPE_UNKNOWN=0, // handled in type checker
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

  kind from_parse(donsus_token_kind type){
    switch(type){
    case DONSUS_BASIC_INT:
      return TYPE_BASIC_INT;

    default : {

    }

    }

  }
  kind type_un;
};

struct NAME_DATA_PAIR{
  DONSUS_TYPE type;
  std::string identifier;
};
#endif