#ifndef DONSUS_FILE_H
#define DONSUS_FILE_H
#include <iostream>

std::string handle_file(const char* path_base);
std::size_t get_file_size(std::ifstream &file);
#endif // DONSUS_FILE_H
