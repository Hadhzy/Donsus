// pratt parser
#include <iostream>
#include "../Include/parser.h"
#include "../Include/donsus.h"
#include "../Include/tree.h"
#include <memory>

// HELPER
std::unique_ptr<donsus_ast> add_to_result(std::unique_ptr<donsus_ast> current, std::unique_ptr<donsus_ast> result){
}


// DEBUG
static std::ostream& operator<<(std::ostream &o, donsus_token& token){
    o << "Value: " << token.value << "\n";
    o << "Kind: " << de_get_name_from_token(token.kind) << "\n";
    o << "Length: " << token.length << "\n";
    o << "Line: " << token.line << "\n";
     return o;
 }

DonsusParser::DonsusParser(donsus_lexer& lexer): error(false), lexer(lexer) {
    cur_token = donsus_lexer_next(*this);
}

donsus_token DonsusParser::donsus_parser_next() {
    return cur_token = donsus_lexer_next(*this);
}

// Todo: remove this from class
void DonsusParser::print_token() {
    while(cur_token.kind != DONSUS_END) {
        std::cout << cur_token << "\n"; // TODO: Implement this properly
        donsus_parser_next();
    }
}

donsus_token DonsusParser::donsus_peek() {
    donsus_lexer save = lexer;
    donsus_token result = donsus_lexer_next(*this);
    lexer = save;
    return result;
}

// TODO: proper debug for lexer and AST
std::unique_ptr<donsus_ast> DonsusParser::donsus_parse() {
    #ifdef DEBUG
    std::cout << "LEXER: " << "\n";
    DonsusParser save = *this;
    print_token();
    *this = save;
    #endif

    std::unique_ptr<donsus_ast> result;

    while (cur_token.kind != DONSUS_END) {
        switch(cur_token.kind){
            case DONSUS_BOOL:
            case DONSUS_VOID:
            case DONSUS_CHAR:
            case DONSUS_BASIC_INT:

            case DONSUS_I8:
            case DONSUS_I16:
            case DONSUS_I32:
            case DONSUS_I64:
            case DONSUS_U32:
            case DONSUS_U64: {
                return donsus_number_expr(cur_token.kind);
            }
            default: {}
        }
    }
    // #ifdef DEBUG
    // std::cout << "AST: " << "\n";
    // // print_ast(result);
    // #endif

    return result;

}

std::unique_ptr<donsus_ast> DonsusParser::donsus_number_expr(unsigned int ptp) {
    std::unique_ptr<donsus_ast> left; // declare the type of left
    std::unique_ptr<donsus_ast> right; // declare the type of right
    // Gt the integer on the left
    left = donsus_number_primary(); // return node with value

    donsus_token previous_token = cur_token;

    if (previous_token.kind == DONSUS_END){
        return left;
    }

    while (previous_token.precedence > ptp) {
       donsus_parser_next(); // get next token
        right = donsus_number_expr(previous_token.precedence); // recursive call
    
        left = donsus_make_ast_node(previous_token, std::move(left), std::move(right), previous_token.kind);

        if(cur_token.kind == DONSUS_END) {
            return left;
        }
    }
    return left;
}

std::unique_ptr<donsus_ast> DonsusParser::donsus_number(donsus_token_kind type){
        std::unique_ptr<donsus_ast> n;  // (value, nullptr, nullptr)
        n = donsus_make_ast_leaf(cur_token, type);
        donsus_parser_next(); // get next token
        return n;
}


std::unique_ptr<donsus_ast> DonsusParser::donsus_number_primary(){
    std::unique_ptr<donsus_ast> n; // declare the type of n
    return donsus_number(cur_token.kind);
}