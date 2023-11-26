cmake_minimum_required(VERSION 3.14)

project(ml-lib LANGUAGES CXX)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network WebSockets)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../doodle-lib/external)

enable_testing()
add_subdirectory(test)

add_library(ml-lib SHARED
  ml-lib_global.h
  #mllib.cpp
  #mllib.h
  #agent.h agent.cpp
  #dqn.h dqn.cpp
  doodleenv.h doodleenv.cpp
  #mlview.h mlview.cpp
  mlserver.h mlserver.cpp
  #mlclient.h mlclient.cpp
  #pybindexample.h pybindexample.cpp
  #exampleservice.h exampleservice.cpp
)

target_include_directories(ml-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../doodle-lib/external/include/)
target_link_libraries(ml-lib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::WebSockets)
target_link_libraries(ml-lib PRIVATE doodle-lib box2d)


target_compile_definitions(ml-lib PRIVATE MLLIB_LIBRARY)
