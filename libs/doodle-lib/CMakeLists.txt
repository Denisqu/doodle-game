cmake_minimum_required(VERSION 3.14)

project(doodle-lib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")


#set(LIB_DIR ${CMAKE_SOURCE_DIR}/external)
#find_library(LIB_NAME NAMES box2d)
#link_directories(doodle-lib external/)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

message(PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(doodle-lib SHARED
  doodle-lib_global.h
  doodlelib.cpp
  doodlelib.h
  view.h view.cpp
  gamescene.h gamescene.cpp
  animatedsprite.h animatedsprite.cpp
  spriteanimator.h spriteanimator.cpp
  gamelogic.h gamelogic.cpp
  entity.h entity.cpp
  entityrenderer.h entityrenderer.cpp
  entityconstructor.h entityconstructor.cpp
  playerentity.h playerentity.cpp
  contactlistener.h contactlistener.cpp
)

target_link_directories(doodle-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)

#set_target_properties(doodle-lib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/box2d.lib)

target_include_directories(doodle-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/include/)
target_link_libraries(doodle-lib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(doodle-lib PRIVATE box2d)
#target_link_libraries(doodle-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/box2d.lib)

add_compile_definitions(_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH)
target_compile_definitions(doodle-lib PRIVATE DOODLELIB_LIBRARY)
