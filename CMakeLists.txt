cmake_minimum_required(VERSION 3.27)

project(donsus LANGUAGES CXX)

add_executable(donsus
        src/main.cc
        src/handle_file.cc
        src/parser.cc
        src/lexer.cc
        src/runner.cc
        src/sema.cc
)

target_include_directories(donsus PUBLIC "${PROJECT_SOURCE_DIR}/Include")