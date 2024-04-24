cmake_minimum_required(VERSION 3.22)
project(slodon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_library(slodon_lib STATIC
        src/main.cc
        lib/context.cc)

add_executable(slodon src/main.cc)
target_link_libraries(slodon PUBLIC slodon_lib)
