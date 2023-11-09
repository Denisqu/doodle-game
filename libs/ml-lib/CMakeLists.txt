cmake_minimum_required(VERSION 3.14)

project(ml-lib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ml-lib SHARED
  ml-lib_global.h
  mllib.cpp
  mllib.h
)

target_compile_definitions(ml-lib PRIVATE MLLIB_LIBRARY)
