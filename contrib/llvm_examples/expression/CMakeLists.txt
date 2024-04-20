cmake_minimum_required(VERSION 3.22)

project(expression)

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
        main.cc
)


#[[add_compile_options(-fsanitize=address) # ASAN
add_link_options(-fsanitize=address) # ASAN]]

execute_process(COMMAND llvm-config --libs core irreader support OUTPUT_VARIABLE LLVM_LIBS)
string(STRIP ${LLVM_LIBS} LLVM_LIBS)
target_link_libraries(donsus_lib PUBLIC ${LLVM_LIBS})

set(CMAKE_BUILD_TYPE Debug)

add_executable(donsus main.cc)

llvm_map_components_to_libnames(llvm_libs core irreader support)
target_link_libraries(donsus PUBLIC ${llvm})
target_link_libraries(donsus PUBLIC donsus_lib)

