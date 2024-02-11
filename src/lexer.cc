// Lexer of the Donsus compiler

#include <iostream>
#include "../Include/parser.h"

std::map<std::string, donsus_token_kind> DONSUS_TYPES_LEXER {
        {"int", DONSUS_BASIC_INT},
        {"int8", DONSUS_I8},
        {"int16", DONSUS_I16},
        {"int32", DONSUS_I32},
        {"int64", DONSUS_I64},
        {"u32", DONSUS_U32},
        {"u64", DONSUS_U64},
        {"bool", DONSUS_BOOL},
        {"void", DONSUS_VOID},
        {"char" , DONSUS_CHAR},
};

std::string de_get_name_from_token(donsus_token_kind kind) {
    switch (kind) {
        case DONSUS_NAME: return "DONSUS_NAME";
        case DONSUS_NUMBER: return "DONSUS_NUMBER";
        case DONSUS_STRING: return "DONSUS_STRING";
        case DONSUS_NEWLINE: return "DONSUS_NEWLINE";
        case DONSUS_LPAR: return "DONSUS_LPAR";
        case DONSUS_RPAR: return "DONSUS_RPAR";
        case DONSUS_LSQB: return "DONSUS_LSQB";
        case DONSUS_RSQB: return "DONSUS_RSQB";
        case DONSUS_COLO: return "DONSUS_COLO";
        case DONSUS_COMM: return "DONSUS_COMM";
        case DONSUS_PLUS: return "DONSUS_PLUS";
        case DONSUS_PLUS_EQUAL: return "DONSUS_PLUS_EQUAL";
        case DONSUS_MINUS: return "DONSUS_MINUS";
        case DONSUS_MINUS_EQUAL: return "DONSUS_MINUS_EQUAL";
        case DONSUS_STAR: return "DONSUS_STAR";
        case DONSUS_STAR_EQUAL: return "DONSUS_STAR_EQUAL";
        case DONSUS_SLASH: return "DONSUS_SLASH";
        case DONSUS_SLASH_EQUAL: return "DONSUS_SLASH_EQUAL";
        case DONSUS_LESS: return "DONSUS_LESS";
        case DONSUS_LESS_EQUAL: return "DONSUS_LESS_EQUAL";
        case DONSUS_GREATER: return "DONSUS_GREATER";
        case DONSUS_GREATER_EQUAL: return "DONSUS_GREATER_EQUAL";
        case DONSUS_EQUAL: return "DONSUS_EQUAL";
        case DONSUS_DOUBLE_EQUAL: return "DONSUS_DOUBLE_EQUAL";
        case DONSUS_DOT: return "DONSUS_DOT";
        case DONSUS_PERCENT: return "DONSUS_PERCENT";
        case DONSUS_LBRACE: return "DONSUS_LBRACE";
        case DONSUS_RBRACE: return "DONSUS_RBRACE";
        case DONSUS_CIRCUMFLEX: return "DONSUS_CIRCUMFLEX";
        case DONSUS_EXCLAMATION: return "DONSUS_EXCLAMATION";
        case DONSUS_COMMENT: return "DONSUS_COMMENT";
        case DONSUS_SINGLE_QUOTE : return "DONSUS_SINGLE_QUOTE";
        case DONSUS_DOUBLE_QUOTE : return "DONSUS_DOUBLE_QUOTE";
        case DONSUS_THREE_DOTS: return "DONSUS_THREE_DOTS";
        case DONSUS_DECREMENT: return "DONSUS_DECREMENT";
        case DONSUS_INCREMENT: return "DONSUS_INCREMENT";
        case DONSUS_BASIC_INT: return "DONSUS_BASIC_INT";
        case DONSUS_I8: return "DONSUS_I8";
        case DONSUS_I16: return "DONSUS_I16";
        case DONSUS_I32: return "DONSUS_I32";
        case DONSUS_I64: return "DONSUS_I64";
        case DONSUS_U32: return "DONSUS_U32";
        case DONSUS_U64: return "DONSUS_U64";
        case DONSUS_BOOL: return "DONSUS_BOOL";
        case DONSUS_VOID: return "DONSUS_VOID";
        case DONSUS_CHAR: return "DONSUS_CHAR";
        case DONSUS_NULL_VALUE: return "DONSUS_NULL_VALUE";
        case DONSUS_END: return "DONSUS_END";


        default:
            return "UNKNOWN_TOKEN_KIND";
    }
}

static bool isstart_identifier(char c){
    // entry point of an identifier
    return isalpha(c) || c == '_';
}

static bool is_type(std::string& s){
    if(DONSUS_TYPES_LEXER.find(s) != DONSUS_TYPES_LEXER.end()){
        return true;
    }
    return false;
}


static bool iscontinue_identifier(char c){
    // continue point of an identifier
    return isstart_identifier(c) || isdigit(c);
}


/*donsus_lexer peak_for_token(donsus_parser * parser){
    donsus_lexer result;
    donsus_lexer old_lexer = parser->lexer;
    donsus_lexer new_token(parser->lexer.string);

    result = new_token;
    parser->lexer = old_lexer;

    return result;
}*/

char peak_for_char(DonsusParser& parser){
    char next_char = parser.lexer.string[parser.lexer.cur_pos + 1];

    if (next_char == '\0') {
        return '\0';
    }

    return next_char;
}

bool eat(DonsusParser& parser){
    if((parser.lexer.cur_char = parser.lexer.string[parser.lexer.cur_pos]) != '\0') {
        std::cout << "here?";
        return true;

    }
    std::cout << "or here?";
    return false;
}

static std::string get_text_between_pos(DonsusParser& parser, unsigned int start, unsigned int end) {
    // returns string from the starting point to the end
    std::cout << "start: " << start;
    std::cout << "end: " << end;

    return {std::begin(parser.lexer.string) + start, std::begin(parser.lexer.string) + end};

}

static std::string next_number(DonsusParser& parser, donsus_token token, unsigned int start_pos) {
    // TBD: we are checking isdigit 2 times here. We can't eat.

    while (isdigit(parser.lexer.cur_char)){
        token.length++;
        eat(parser);
    };
    return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
};

static std::string next_identifier(DonsusParser& parser, donsus_token token, unsigned int start_pos){
    while (iscontinue_identifier(parser.lexer.cur_char)){
        token.length++;
        eat(parser);
    }
    return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
}

static donsus_token make_type(DonsusParser& parser, std::string& value, unsigned int length){
    // construct type token
    donsus_token token;
    token.line = parser.lexer.cur_line;
    token.kind = DONSUS_TYPES_LEXER[value];
    token.length = length;
    token.value = value;

    return token;
}

void consume_spaces(DonsusParser& parser){
    while (std::isspace(parser.lexer.cur_char)){
        eat(parser);
    }
}

donsus_token donsus_lexer_next(DonsusParser& parser) {
    donsus_token cur_token{DONSUS_END, "", 0, parser.lexer.cur_line}; // aggregate initialisation


    // consume spaces
    consume_spaces(parser);

    switch (parser.lexer.cur_char) {
        case '\n': {
            cur_token.line = ++parser.lexer.cur_line;
            cur_token.kind = DONSUS_NEWLINE;
            cur_token.length = 1;
            eat(parser);
            return cur_token;
        }
        case '+': {
            cur_token.precedence = 10; // lower precedence
            cur_token.kind = DONSUS_PLUS;
            cur_token.length = 1;
            cur_token.value = "+";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case '-': {
            cur_token.precedence = 10; // lower precedence
            cur_token.kind = DONSUS_MINUS;
            cur_token.length = 1;
            cur_token.value = "-";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case '*': {
            cur_token.precedence = 20; // higher precedence
            cur_token.kind = DONSUS_STAR;
            cur_token.length = 1;
            cur_token.value = "*";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case '/': {
            cur_token.precedence = 20; // higher precedence
            cur_token.kind = DONSUS_SLASH;
            cur_token.length = 1;
            cur_token.value = "/";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case '^': {
            cur_token.kind = DONSUS_CIRCUMFLEX;
            cur_token.length = 1;
            cur_token.value = "^";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case '=': {
            cur_token.kind = DONSUS_EQUAL;
            cur_token.length = 1;
            cur_token.value = "=";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case '(': {
            cur_token.kind = DONSUS_LPAR;
            cur_token.length = 1;
            cur_token.value = "(";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case ')': {
            cur_token.kind = DONSUS_RPAR;
            cur_token.length = 1;
            cur_token.value = ")";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }

        case ';': {
            cur_token.kind = DONSUS_SEMICOLON;
            cur_token.length = 1;
            cur_token.value = ";";
            cur_token.line = parser.lexer.cur_line;
            eat(parser);
            return cur_token;
        }
        default: {
            // numbers
            if (isdigit(parser.lexer.cur_char)) {
                cur_token.kind = DONSUS_NUMBER;
                cur_token.length = 0; // will be changed during next_number
                cur_token.value = next_number(parser, cur_token, parser.lexer.cur_pos);
                cur_token.line = parser.lexer.cur_line;
                return cur_token;
            }

            // identifiers
            if (isstart_identifier(parser.lexer.cur_char)) {
                cur_token.kind = DONSUS_NAME;
                cur_token.length = 0; // will be changed during next_number
                static std::string c_value = next_identifier(parser, cur_token, parser.lexer.cur_pos);

                // If it is a type
                if(is_type(c_value)){
                    return make_type(parser, c_value, cur_token.length);
                }
                else {
                    cur_token.value = c_value;  // default choice
                }

                cur_token.line = parser.lexer.cur_line;
                return cur_token;
            }

            // if none of them are true we end the execution.(error|end of the file)
            return cur_token;
        }
    }
}