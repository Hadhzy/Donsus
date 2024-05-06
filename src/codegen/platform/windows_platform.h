#ifndef DONSUS_WINDOWS_PLATFORM_H
#define DONSUS_WINDOWS_PLATFORM_H
#include "../src/codegen/bitness.h"
#include <filesystem>
#include <iostream>
#include <vector>

class WindowsPlatform {
public:
  std::string GetObjectFIleExtension() { return ""; }
  std::string GetExeFileExtension() { return ""; }

  std::string
  GetLinkerCommand(const std::vector<std::filesystem::path> &obj_paths,
                   const std::filesystem::path &exe_path,
                   Bitness bitness) const {
 /*   switch(bitness){
    case Bitness::
    }*/
  }
};
#endif