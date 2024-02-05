#include <iostream>
#include "../Include/parser.h"
#include "../Include/donsus.h"
#include "../Include/tree.h"
#include "./utils/ast_graph_generator.cc"
#include <memory>

std::unique_ptr<donsus_ast> donsus_expr(donsus_parser& parser, unsigned int ptp);
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
    // Graph the AST tree 
    ASTVisualiserGraph graph;
    graph.donsus_generateGraph(std::move(result));  // Pass the unique_ptr by value (not using std::move)
    #endif

    return result;
}

// Return an AST tree
// Parameter ptp refers to the "previous token's precedence"
std::unique_ptr<donsus_ast> donsus_expr(donsus_parser& parser, unsigned int ptp) {
    std::unique_ptr<donsus_ast> left; // declare the type of left
    std::unique_ptr<donsus_ast> right; // declare the type of right
    // Gt the integer on the left
    left = donsus_primary(parser); // return node with value

    donsus_token cur_token = parser.cur_token; // obtain cur_token
    if (cur_token.kind == DONSUS_END){
        return left;
    }

    while (cur_token.precedence > ptp) {
       donsus_parser_next(parser); // get next token
        right = donsus_expr(parser, cur_token.precedence); // recursive call
    
        left = donsus_make_ast_node(cur_token, std::move(left), std::move(right), cur_token.kind);

        if(parser.cur_token.kind == DONSUS_END) {
            return left;
        }
    }
    return left;
}

std::unique_ptr<donsus_ast> donsus_number(donsus_parser& parser, donsus_token_kind type){
        std::unique_ptr<donsus_ast> n;  // (value, nullptr, nullptr)
        n = donsus_make_ast_leaf(parser.cur_token, type);
        donsus_parser_next(parser); // get next token
        return n;
}
std::unique_ptr<donsus_ast> donsus_primary(donsus_parser& parser){
    std::unique_ptr<donsus_ast> n; // declare the type of n

    switch(parser.cur_token.kind) {
        // handle basic number
        case DONSUS_NUMBER:
            return donsus_number(parser, DONSUS_BASIC_INT);

        // handle integers
        case DONSUS_BASIC_INT:
        case DONSUS_I8:
        case DONSUS_I16:
        case DONSUS_I32:
        case DONSUS_I64:
        case DONSUS_U32:
        case DONSUS_U64: {
            return donsus_number(parser, parser.cur_token.kind);
         }

        case DONSUS_BOOL:
        case DONSUS_VOID:
        case DONSUS_CHAR:
        default:
            std::cout << "syntax error somewhere";
    }
}

