#ifndef DONSUS_LINUX_PLATFORM_H
#define DONSUS_LINUX_PLATFORM_H
#include "../src/codegen/bitness.h"
#include <filesystem>
#include <iostream>
#include <vector>

class LinuxPlatform {
public:
  std::string GetObjectFileExtension() const { return ".o"; }
  std::string GetExeFileExtension() const { return ""; }

  std::string
  GetLinkerCommand(const std::vector<std::filesystem::path> &obj_paths,
                   const std::filesystem::path &exe_path,
                   Bitness bitness) const {
    switch (bitness) {
    case Bitness::x64:
      break;
    case Bitness::x32: {
      std::cerr << "32 bit is not support on linux, only 64 bit is supported";
      abort();
    }
    }
    std::string obj_input_linker_arg;

    // output.o
    for (const auto &obj_path : obj_paths) {
      obj_input_linker_arg += obj_path.string() + GetObjectFileExtension();
      obj_input_linker_arg += "";
    }

#if defined(DU_COMPILER_GCC)
    std::string linker_cmd = "gcc ";
#elif defined(DU_COMPILER_CLANG)
    std::string linker_cmd = "clang ";
#endif
    linker_cmd += obj_input_linker_arg;

    return linker_cmd;
  }
};

#endif