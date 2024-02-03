#include <iostream>
#include "../Include/parser.h"
#include "../Include/donsus.h"

// forward references
donsus_ast& donsus_expr(donsus_parser& parser, donsus_ast& cur_state);
donsus_ast& donsus_term(donsus_parser& parser, donsus_ast& cur_state);
donsus_ast& donsus_factor(donsus_parser& parser, donsus_ast& cur_state);

donsus_token donsus_parser_next(donsus_parser& parser){
    return parser.cur_token = donsus_lexer_next(parser);
}

// DEBUG
static std::ostream& operator<<(std::ostream &o, donsus_token& token){
    o << "Value: " << token.value << "\n";
    o << "Kind: " << de_get_name_from_token(token.kind) << "\n";
    o << "Length: " << token.length << "\n";
    o << "Line: " << token.line << "\n";
    return o;
}

// DEBUG
static
void print_token(donsus_parser& parser){
    while (parser.cur_token.kind != DONSUS_END){
        std::cout << parser.cur_token << "\n";
        donsus_parser_next(parser); // DO this without moving the actual state of the parser
    }
}

// DEBUG
static
void print_ast(donsus_ast& result) {
    auto *temp = result.left;
    while (temp){
        std::cout << '\n' << result.left->value; // call custom operator <<
        if (temp->left){
            temp = temp->left;
        } else {
            break;
        }
    }

    temp = result.right;
    while (temp){
        std::cout << '\n' << result.right->value; // call custom operator <<
        if (temp->right){
            temp = temp->right;
        } else {
            break;
        }
    }
}
// start parsing process
donsus_ast& donsus_parse(donsus_parser& parser, donsus_ast& base){

    #ifdef DEBUG
    std::cout << "LEXER: " << "\n";
    donsus_parser save = parser;
    print_token(parser);
    parser = save;
    #endif

    donsus_ast& result = donsus_expr(parser, base);

    #ifdef DEBUG
    std::cout << "AST: " << "\n";
    print_ast(result);
    #endif

    return result;
}

donsus_token donsus_peek(donsus_parser& parser){
    donsus_lexer save = parser.lexer;
    donsus_token result = donsus_lexer_next(parser);
    parser.lexer = save;
    return result;
}

donsus_ast& donsus_expr(donsus_parser& parser, donsus_ast& cur_state){
    donsus_ast& left = donsus_term(parser, cur_state);
    if (donsus_peek(parser).kind != DONSUS_PLUS && donsus_peek(parser).kind != DONSUS_MINUS){
        // fast return -> result | terminal
        return cur_state;
    }

    cur_state.left = &left;
    donsus_parser_next(parser);

    while(parser.cur_token.kind == DONSUS_PLUS || parser.cur_token.kind == DONSUS_MINUS) {
        switch (parser.cur_token.kind) {
            case DONSUS_PLUS: {
                donsus_parser_next(parser); // skip DONSUS_PLUS
                donsus_ast& right = donsus_term(parser, cur_state);
                cur_state.right = &right;
                donsus_parser_next(parser); // skip DONSUS_MINUS
                // make + token
                donsus_token plus_token = {.kind = DONSUS_PLUS, .value = "+", .length = 1, .line = parser.lexer.cur_line};
                cur_state.value = plus_token;
                break;
            }

            case DONSUS_MINUS: {
                donsus_parser_next(parser); // skip DONSUS_MINUS
                donsus_ast& right = donsus_term(parser, cur_state);
                cur_state.right = &right;
                donsus_parser_next(parser); // skip DONSUS_MINUS
                // make -  token
                donsus_token minus_token = {.kind = DONSUS_MINUS, .value = "-", .length = 1, .line = parser.lexer.cur_line};
                cur_state.value = minus_token;
                break;
            }

            default: break;
        }
    }

    return donsus_expr(parser, cur_state); // start recursive call
}

donsus_ast& donsus_term(donsus_parser& parser, donsus_ast& cur_state){
    donsus_ast& left = donsus_factor(parser, cur_state);
    if (donsus_peek(parser).kind != DONSUS_STAR && donsus_peek(parser).kind != DONSUS_SLASH){
        return left;
    }

    cur_state.left = &left;
    donsus_parser_next(parser);

    while(parser.cur_token.kind == DONSUS_STAR || parser.cur_token.kind == DONSUS_SLASH){
        switch(parser.cur_token.kind){
            case DONSUS_STAR: {
                donsus_parser_next(parser);
                donsus_ast& right = donsus_factor(parser, cur_state);
                cur_state.right = &right;
                donsus_parser_next(parser); // skip DONSUS_MINUS
                donsus_token star_token = {.kind = DONSUS_STAR, .value = "*", .length = 1, .line = parser.lexer.cur_line};
                cur_state.value = star_token;
                break;
            }
            case DONSUS_SLASH: {
                donsus_parser_next(parser);
                donsus_ast& right = donsus_factor(parser, cur_state);
                cur_state.right = &right;
                donsus_parser_next(parser); // skip DONSUS_MINUS
                donsus_token slash_token = {.kind = DONSUS_SLASH, .value = "/", .length = 1, .line = parser.lexer.cur_line};
                cur_state.value = slash_token;
                break;
            }
            default: break;
        }
    }
    return cur_state;
}

donsus_ast& donsus_factor(donsus_parser& parser, donsus_ast& cur_state){
    auto token = parser.cur_token;
    auto *factor = new donsus_ast();

    if (token.kind == DONSUS_LPAR){
        donsus_parser_next(parser);
        donsus_ast& result = donsus_expr(parser, cur_state);
        factor->left = &result;
        donsus_parser_next(parser);

        if (parser.cur_token.kind != DONSUS_RPAR){
            throw std::invalid_argument("Mismatched parentheses");
        }
        return result;
    }
    factor->value = token;
    return *factor;
}
