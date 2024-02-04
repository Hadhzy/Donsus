#include <iostream>
#include "../Include/parser.h"
#include "../Include/donsus.h"
#include "../Include/tree.h"
#include <memory>

std::unique_ptr<donsus_ast> donsus_expr(donsus_parser& parser, int ptp);
std::unique_ptr<donsus_ast> donsus_primary(donsus_parser& parser);


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

static
void print_token(donsus_parser& parser){
    while (parser.cur_token.kind != DONSUS_END){
        std::cout << parser.cur_token << "\n";
        donsus_parser_next(parser); // DO this without moving the actual state of the parser
    }
}

// print ast
// static
// void print_ast(donsus_ast& result) {
//     std::unique_ptr<donsus_ast> temp = result.left;
//     while (temp){
//         std::cout << '\n' << result.left->value; // call custom operator <<
//         if (temp->left){
//             temp = temp->left;
//         } else {
//             break;
//         }
//     }

//     temp = result.right;
//     while (temp){
//         std::cout << '\n' << result.right->value; // call custom operator <<
//         if (temp->right){
//             temp = temp->right;
//         } else {
//             break;
//         }
//     }
// }


// peek
donsus_token donsus_peek(donsus_parser& parser){
    donsus_lexer save = parser.lexer;
    donsus_token result = donsus_lexer_next(parser);
    parser.lexer = save;
    return result;
}

// Starting point
std::unique_ptr<donsus_ast> donsus_parse(donsus_parser& parser, donsus_ast& base){
    
    #ifdef DEBUG
    std::cout << "LEXER: " << "\n";
    donsus_parser save = parser;
    print_token(parser);
    parser = save;
    #endif

    std::unique_ptr<donsus_ast> result = donsus_expr(parser, 0);

    #ifdef DEBUG
    std::cout << "AST: " << "\n";
    // print_ast(result);
    #endif

    return result;
}

// Return an AST tree
// Parameter ptp refers to the "previous token's precedence"
std::unique_ptr<donsus_ast> donsus_expr(donsus_parser& parser, const int ptp) {
    std::unique_ptr<donsus_ast> left;
    std::unique_ptr<donsus_ast> right;
    // Gt the integer on the left
    left = donsus_primary(parser);

    donsus_token cur_token = parser.cur_token;
    if (cur_token.kind == DONSUS_END){
        return left;
    }

    while (cur_token.precedence > ptp) {
       donsus_parser_next(parser);
        right = donsus_expr(parser, cur_token.precedence);
    
        left = make_ast_node(cur_token, std::move(left), std::move(right));

        if(parser.cur_token.kind == DONSUS_END) {
            return left;
        }
    }

    return left;
}

std::unique_ptr<donsus_ast> donsus_primary(donsus_parser& parser){
    std::unique_ptr<donsus_ast> n = std::make_unique<donsus_ast>();

    switch(parser.cur_token.kind) {
        case DONSUS_NUMBER:
            n = make_ast_leaf(parser.cur_token);
            donsus_parser_next(parser);
            return n;
        
        default: 
            std::cout << "syntax error somewhere";
    }
}

