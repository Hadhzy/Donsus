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
        parser/test_variables.cc
        parser/test_functions.cc
        parser/test_if_statements.cc
        parser/test_expressions.cc
        parser/test_function_call.cc
        parser/test_return.cc
        parser/test_assignments.cc
        parser/test_print.cc

        # typecheck
        typecheck/test_return.cc
        typecheck/test_variables.cc
        typecheck/test_function.cc
        typecheck/test_assignment_type.cc
        typecheck/test_function_call_args.cc
        typecheck/test_function_call_rvalue.cc
        typecheck/test_identifier_rvalue.cc
        typecheck/test_if_statements_type.cc
       

        #symbol table
        symbol_table/test_variables.cc
        symbol_table/test_sym_functions.cc
)


include(GoogleTest)
gtest_discover_tests(donsus_test)

target_link_libraries(donsus_test PUBLIC
        GTest::gtest_main
        ${llvm}
        donsus_lib
)