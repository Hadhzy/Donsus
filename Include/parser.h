#include <iostream>
#include <memory>

#ifndef PARSER_H
#define PARSER_H

typedef enum{
    // Tokens
    DONSUS_NAME, // IDENTIFIER
    DONSUS_NUMBER, // 69
    DONSUS_STRING, // "hello world"
    DONSUS_NEWLINE, // \n
    DONSUS_END, // mark the end of the code

    DONSUS_LPAR, // (
    DONSUS_RPAR, // )
    DONSUS_LSQB, // [
    DONSUS_RSQB, // ]
    DONSUS_COLO, // :
    DONSUS_COMM,  // ,
    DONSUS_PLUS, // +
    DONSUS_PLUS_EQUAL, // +=
    DONSUS_MINUS, // -
    DONSUS_MINUS_EQUAL, // -=
    DONSUS_STAR, // *
    DONSUS_STAR_EQUAL, // *=
    DONSUS_SLASH, // /
    DONSUS_SLASH_EQUAL, // /=
    DONSUS_LESS, // x < y
    DONSUS_LESS_EQUAL, // x <= y
    DONSUS_GREATER, // x > y
    DONSUS_GREATER_EQUAL, // x >= y
    DONSUS_EQUAL, // =
    DONSUS_DOUBLE_EQUAL, // ==
    DONSUS_DOT, // .
    DONSUS_PERCENT, // %
    DONSUS_LBRACE, // {
    DONSUS_RBRACE, // }
    DONSUS_CIRCUMFLEX, // ^
    DONSUS_INCREMENT, // ++
    DONSUS_DECREMENT, // --
    DONSUS_EXCLAMATION, // !
    DONSUS_COMMENT, // #
    DONSUS_SINGLE_QUOTE, // '
    DONSUS_DOUBLE_QUOTE, // "
    DONSUS_THREE_DOTS, // ...
    DONSUS_NULL_VALUE,

    // INT
    DONSUS_BASIC_INT, // int
    DONSUS_I8, // FROM -(2^7) TO (2^7) -1
    DONSUS_I16, // FROM −(2^15) TO 2^15 − 1
    DONSUS_I32, // FROM 0 TO 2^32 -1
    DONSUS_I64, // FROM 0 TO 2^64 − 1
    DONSUS_U32, // FROM 0 TO 2^32 -1
    DONSUS_U64, // FROM 0 TO 2^64 − 1

    DONSUS_BOOL, // bool
    DONSUS_VOID, // void
    DONSUS_CHAR, // 'D'
} donsus_token_kind;

struct donsus_token{
    donsus_token_kind  kind;
    std::string value;
    unsigned int length;
    unsigned int line;
};

// AST implementation
struct donsus_ast{
    donsus_token value;
    donsus_ast *left = nullptr;
    donsus_ast *right = nullptr;
};

struct donsus_lexer {
    donsus_lexer(std::string input) :
    string(input), cur_pos(0), cur_line(1), cur_char(input[0]) {}
    donsus_lexer() = default;

    std::string string;
    char cur_char;
    unsigned int cur_pos, cur_line;
};

struct donsus_parser;
donsus_token donsus_lexer_next(donsus_parser& parser); // forward reference

struct donsus_parser{
    bool error;
    donsus_token cur_token;
    donsus_lexer lexer;
};

/*donsus_lexer donsus_new_lexer(std::string string);*/
void de_printout_single_token(donsus_token);
std::string de_get_name_from_token(donsus_token_kind kind);
donsus_parser& donsus_parser_init(donsus_lexer& lexer);
donsus_token donsus_parser_next(donsus_parser &parser);
donsus_ast& donsus_parse(donsus_parser& parser, donsus_ast& base);
#endif // PARSER_H