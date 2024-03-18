cmake_minimum_required(VERSION 3.22)

project(donsus_test)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(donsus_test
                test_variables.cc
                test_functions.cc
                test_if_statements.cc
                test_expressions.cc
                test_function_call.cc
                test_return.cc
                test_assignments.cc)


include(GoogleTest)
gtest_discover_tests(donsus_test)

target_link_libraries(donsus_test PUBLIC
                        GTest::gtest_main
                        donsus_lib
                        )
