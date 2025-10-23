cmake_minimum_required(VERSION 3.14)
project(second_laba)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(run_file main.cpp)

add_executable(tests tests.cpp)
target_link_libraries(tests GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME MyProjectTests COMMAND tests)