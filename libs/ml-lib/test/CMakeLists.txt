cmake_minimum_required(VERSION 3.14)

project(ml-lib-tests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/googletest-1.14.0)

add_executable(ml-lib-tests mllibtest.cpp)
target_link_libraries(ml-lib-tests PUBLIC
    gtest_main
    ml-lib
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt on)

add_test(
    NAME ml-lib-tests
    COMMAND ml-lib-tests
)
