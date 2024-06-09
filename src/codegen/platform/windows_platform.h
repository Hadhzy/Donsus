#ifndef DONSUS_WINDOWS_PLATFORM_H
#define DONSUS_WINDOWS_PLATFORM_H
#include "../src/codegen/bitness.h"
#include <filesystem>
#include <iostream>
#include <vector>


// Todo: Have a script which locates vcvarsall.bat and sets up the environment
// Todo: Assume MSVC by default 
class WindowsPlatform {
public:
    std::string GetObjectFileExtension() const { return ".obj"; }
    std::string GetExeFileExtension() const { return ".exe"; }
    // Todo: don't hardcode the path
    std::string LinkLibs() const { return R"(C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64)"; }
    
    std::string
        GetLinkerCommand(const std::vector<std::filesystem::path>& obj_paths,
            const std::filesystem::path& exe_path,
            Bitness bitness) const {
        std::string obj_input_linker_arg;
        for (const auto& obj_path : obj_paths) {
            obj_input_linker_arg += obj_path.string() + GetObjectFileExtension() + " ";
        }

        std::string linker_cmd = "link /OUT:" + exe_path.string() + " " + obj_input_linker_arg + LinkLibs();
        switch (bitness) {
        case Bitness::x64: {
            linker_cmd += "/MACHINE:X64";
            break;
        }
        case Bitness::x32: {
            linker_cmd += " /MACHINE:X86";
            break;
        }
        default: {
            std::cerr << "Unsupported bitness";
            abort();
        }

        }
        return linker_cmd;
    };
};
#if defined(DU_SYSTEMS_WINDOWS)
#error "Not fully supported yet"
#endif

#endif