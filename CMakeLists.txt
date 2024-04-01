cmake_minimum_required(VERSION 3.22)
project(donsus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find LLVM
execute_process(COMMAND llvm-config --cmakedir OUTPUT_VARIABLE LLVM_CMAKE_DIR)
string(STRIP ${LLVM_CMAKE_DIR} LLVM_CMAKE_DIR)
list(APPEND CMAKE_PREFIX_PATH ${LLVM_CMAKE_DIR})
find_package(LLVM REQUIRED CONFIG)

# Set compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_COMPILE_FLAGS}")
set(ENABLE_LLVM_SHARED 1)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

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

        # optimisation
        src/optimisation_ast.cc
)


execute_process(COMMAND llvm-config --libs core irreader support OUTPUT_VARIABLE LLVM_LIBS)
string(STRIP ${LLVM_LIBS} LLVM_LIBS)
target_link_libraries(donsus_lib PUBLIC ${LLVM_LIBS})

set(CMAKE_BUILD_TYPE Debug)

# Add include directories for the library
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include/codegen")
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include/Internal")


add_executable(donsus src/main.cc)

llvm_map_components_to_libnames(llvm_libs core irreader support)
target_link_libraries(donsus PUBLIC ${llvm})
target_link_libraries(donsus PUBLIC donsus_lib)

add_subdirectory(donsus_test)

