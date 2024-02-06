// Generate assembly for x64 arhitecture
// DO NOT WRITE TO FILE MANUALLY(file.write_to_file())
// CODEGEN=ASMGEN
#include "../Include/parser.h"
#include "../Include/symbol_table.h"
#include "../../asm/duasm.h"
#include <iostream>
#include <memory>
#include <vector>

// Forward references
class DATA_INSTRUCTION;

// DECLARATIONS
class NASM_STRING{
public:

    NASM_STRING() = default;

    std::string to_string(std::string& value){
        value_string = "'" + value + "'";
        return value_string;
    }
    private:
        std::string value_string;
};

enum DX{
    DB,
    DW,
    DD,
    DQ,
    DT, // do not accept integer numeric constant
    DO, // do not accept integer numeric constant
    DY, // do not accept integer numeric constant
    DZ // do nto accept integer numeric constant
};

std::string donsus_codegen_get_string_type_DX(DX dx) {
    // obtain string from enum member
    switch(dx) {
        case DB: return "db";
        case DW: return "dw";
        case DD: return "dd";
        case DQ: return "dq";
        case DT: return "dt";
        case DO: return "do";
        case DY: return "dy";
        case DZ: return "dz";
    }
}

// NASM SECTION
class SECTION{
};

// Todo: gather all stuff and then call fetch on every instruction
// DATA SEGMENT
class DATA: public SECTION{
    // Initialised data(dx)
public:
    DATA(AsmFile& file, DATA_INSTRUCTION& child){
        file.create_section("data");
        ins.push_back(child); // add child
        COUNTER_INS++; // INCREASE INS
    }
private:
    int COUNTER_INS = 0; // COUNTER OF INSTRUCTIONS
    std::vector<DATA_INSTRUCTION> ins;
};

// TEXT SEGMENT
class TEXT: public SECTION{

};

class DATA_INSTRUCTION: public DATA{
public:
    // Instruction in a data segment
    // (name, data type, value, new_line)
    DATA_INSTRUCTION(AsmFile& file): DATA(file, *this){}
    std::string name; // name of the var
    DX type; // "DX"

    std::string value; // actual value
    NASM_STRING string; // string view

    std::string new_line; // NEW_LINE
    void handle_name(AsmFile& file) {
        file.write_to_file(name);
    }
    void handle_type(AsmFile& file){
        file.write_to_file(" " + donsus_codegen_get_string_type_DX(type));
    }
    void handle_string(AsmFile& file){
        file.write_to_file(string.to_string(value));
    }
    void handle_new_line(AsmFile& file){
        file.write_to_file("09AH", true); // new line
    }

    void fetch(AsmFile& file){
        // Add the actual instruction to the file
        handle_name(file);
        handle_type(file);
        handle_string(file);
        handle_new_line(file);
    }
};

// GLOBALS
void donsus_codegen_create_instructions(){

}

void donsus_codegen_symbol(donsus_symbol& symbol, AsmFile& file){

}

// START CODEGEN
void donsus_codegen_section_vars_sym(donsus_symtable* symtable, AsmFile& file){
    // find out variables from the symbol table
    if (!symtable->get_sym().empty()) return; // there are no symbols

    auto inside_sym = [](const auto& key, const auto& value, AsmFile& file){

    };
        for (const auto& n: symtable->get_sym()){
        inside_sym(n.first, n.second, file);
    }
}

void donsus_codegen_create_data_section(donsus_ast* ast, donsus_symtable* symtable, AsmFile& file){
    /*DATA data_sec;*/
    // add stuff to data section from symbol table
    donsus_codegen_section_vars_sym(symtable, file);
}

// Enter point
void donsus_codegen_x64(std::unique_ptr<donsus_ast> ast, std::unique_ptr<donsus_symtable> symtable, std::string& filename){

    AsmFile x64_file(filename); // create file instance

    // create data section
    donsus_codegen_create_data_section(ast.get(), symtable.get(), x64_file);
}
