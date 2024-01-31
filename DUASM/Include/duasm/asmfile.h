// Low level api for creating assembly files (writing, reading)
#ifndef DUASM_ASMFILE_H
#define DUASM_ASMFILE_H
#include <iostream>
#include <fstream>

const std::string file_ext = ".s";

class AsmFile {
    AsmFile(std::string path, bool setup_needed = true);
    ~AsmFile();
    void write_to_file(std::string content, bool new_line_needed = true);
    void create_section(std::string section_name);
    void setup(std::string num1, std::string num2);

private:
    void new_line(unsigned int amount);
    void add_comment(std::string content);
    std::ofstream file;
};

AsmFile::AsmFile(std::string path, bool setup_needed) {
    // Constructor: open up a file
    file.open(path + ext);
    if(setup_needed) setup();
}

AsmFile::~AsmFile(std::string path) {
    // Destructor: close down a file
    file.close();
}

void AsmFile::setup(std::string num1, std::string num2) {
    /
}

void AsmFile::new_line(unsigned int amount) {
    // Add a new line specified by the amount
    for(int i = 0; i < amount; i++) {
        file << "\n";
    }
}
void AsmFile::add_comment(std::string content){
    file << "; " + content;
}

void AsmFile::write_to_file(std::string content, bool new_line_needed) {
    if(new_line_needed) {
        file << content;
        new_line(1);
    } else {
        file << content;
    }
}

void AsmFile:create_section(std::string section_name) {
    //creating a section like .data
    file << "section ." + section_name;
    new_line(1);
}

#endif