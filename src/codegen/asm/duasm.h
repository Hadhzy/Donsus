#ifndef DUASH_FILE_H
#define DUASH_FILE_H
#include <iostream>
#include <fstream>

const std::string file_ext = ".s";

class AsmFile {
public:
    AsmFile(std::string path, bool setup_needed = true);
    ~AsmFile();
    void write_to_file(std::string content, bool new_line_needed = true);
    void create_section(std::string section_name);
    void setup();

private:
    void new_line(unsigned int amount);
    void add_comment(std::string content);
    std::ofstream file;
};

#endif