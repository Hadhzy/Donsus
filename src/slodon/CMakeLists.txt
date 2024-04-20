cmake_minimum_required(VERSION 3.22)
project(slodon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_library(donsus_lib STATIC src/main.cc)

add_executable(donsus src/main.cc)
target_link_libraries(donsus PUBLIC donsus_lib)
