cmake_minimum_required(VERSION 3.22)

project(donsus LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)


add_executable(donsus
        src/main.cc
        src/handle_file.cc
        src/parser.cc
        src/lexer.cc
        src/runner.cc
        src/sema.cc
        src/ir.cc
        src/symbol_table.cc

        # codegen
        src/codegen/asm/duasm.cc
        src/codegen/target/x64/x64.cc

        #utility
        src/utility/handle.h
        src/utility/macros.h
        src/utility/memory_alloc.h
        src/utility/memory_alloc.cc
        src/utility/slices.h


        #ast
        src/ast/node.cc
        src/ast/tree.cc

)

target_include_directories(donsus PUBLIC "${PROJECT_SOURCE_DIR}/Include")