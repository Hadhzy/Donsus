#ifndef DONSUS_FILE_H
#define DONSUS_FILE_H
#include <iostream>
#include "parser.h"

std::string handle_file(const char *path_base);
std::size_t get_file_size(std::ifstream &file);
// Second-highest group in Donsus
struct DonsusAstFile {
  int id;
  int flags;

  std::string fullpath;
  std::string filename;
  std::string directory;

  std::string extension;

  std::size_t error_count;
};
// Highest group in Donsus
struct BuildContext {};
#endif // DONSUS_FILE_H
