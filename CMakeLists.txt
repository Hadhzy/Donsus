cmake_minimum_required(VERSION 3.22)

project(donsus LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)


add_executable(donsus
        src/main.cc
        src/handle_file.cc
        src/parser.cc
        src/lexer.cc
        src/runner.cc
        src/sema.cc
        src/ir.cc

        # symtable
        src/symbol_table.cc

        # codegen

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