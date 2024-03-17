cmake_minimum_required(VERSION 3.22)

add_library(donsus_lib STATIC
        src/handle_file.cc
        src/lexer.cc
        src/runner.cc
        src/sema.cc
        src/ir.cc

        # type
        src/type.cc

        #parser
        src/parser_util.cc
        src/parser.cc

        # symtable
        src/symbol_table.cc

        # codegen
        src/codegen/codegen.cc

        #utility
        src/utility/handle.h
        src/utility/macros.h
        src/utility/memory_alloc.h
        src/utility/memory_alloc.cc
        src/utility/slices.h
        src/utility/exception.cc
        src/utility/exception.h

        #ast
        src/ast/node.cc
        src/ast/tree.cc
)

set(CMAKE_BUILD_TYPE Debug)

target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include/codegen")
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include/Internal")

add_executable(donsus src/main.cc)

target_link_libraries(donsus PUBLIC donsus_lib)

add_subdirectory(donsus_test)

