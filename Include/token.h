#ifndef DONSUS_TOKEN_H
#define DONSUS_TOKEN_H

#include <iostream>

typedef enum {
  // Tokens
  DONSUS_NAME,    // IDENTIFIER
  DONSUS_END,     // mark the end of the code
  DONSUS_NUMBER,  // 69
  DONSUS_STRING,  // "hello world"
  DONSUS_NEWLINE, // \n

  DONSUS_LPAR,          // (
  DONSUS_RPAR,          // )
  DONSUS_LSQB,          // [
  DONSUS_RSQB,          // ]
  DONSUS_COLO,          // :
  DONSUS_COMM,          // ,
  DONSUS_SEMICOLON,     // ;
  DONSUS_PLUS,          // +
  DONSUS_PLUS_EQUAL,    // +=
  DONSUS_MINUS,         // -
  DONSUS_MINUS_EQUAL,   // -=
  DONSUS_STAR,          // *
  DONSUS_STAR_EQUAL,    // *=
  DONSUS_SLASH,         // /
  DONSUS_SLASH_EQUAL,   // /=
  DONSUS_LESS,          // x < y
  DONSUS_LESS_EQUAL,    // x <= y
  DONSUS_GREATER,       // x > y
  DONSUS_GREATER_EQUAL, // x >= y
  DONSUS_EQUAL,         // =
  DONSUS_DOUBLE_EQUAL,  // ==
  DONSUS_DOT,           // .
  DONSUS_PERCENT,       // %
  DONSUS_LBRACE,        // {
  DONSUS_RBRACE,        // }
  DONSUS_CIRCUMFLEX,    // ^
  DONSUS_INCREMENT,     // ++
  DONSUS_DECREMENT,     // --
  DONSUS_EXCLAMATION,   // !
  DONSUS_COMMENT,       // #
  DONSUS_SINGLE_QUOTE,  // '
  DONSUS_THREE_DOTS,    // ...
  DONSUS_NULL_VALUE,

  // INT
  DONSUS_BASIC_INT, // int
  DONSUS_I8,        // [FROM -(2^7) TO (2^7) -1]
  DONSUS_I16,       // [FROM −(2^15) TO 2^15 − 1]
  DONSUS_I32,       // [FROM 0 TO 2^32 -1]
  DONSUS_I64,       // [FROM 0 TO 2^64 − 1]
  DONSUS_U32,       // [FROM 0 TO 2^32 -1]
  DONSUS_U64,       // [FROM 0 TO 2^64 − 1]

  DONSUS_BOOL, // bool
  DONSUS_VOID, // void
  DONSUS_CHAR, // 'D'

  // keywords
  DONSUS_FUNCTION_DEFINITION_KW // def
} donsus_token_kind;

struct donsus_token {
  donsus_token_kind kind;  // the kind of the token
  std::string value;       // the value of the token(string)
  unsigned int length;     // the length of the token
  unsigned int line;       // the line number of the token(starts from one)
  unsigned int precedence; // precedence of the token(the higher the value the
                           // more precedence the token has)
};

#endif