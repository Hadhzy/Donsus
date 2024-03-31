cmake_minimum_required(VERSION 3.22)


find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

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


include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs all)

set(CMAKE_BUILD_TYPE Debug)

#[[
add_compile_options(-fsanitize=address) # ASAN
add_link_options(-fsanitize=address) # ASAN
]]

target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include/codegen")
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include/Internal")

add_executable(donsus src/main.cc)

target_link_libraries(donsus PUBLIC ${llvm_libs})
target_link_libraries(donsus PUBLIC donsus_lib)


add_subdirectory(donsus_test)

