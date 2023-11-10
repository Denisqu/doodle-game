cmake_minimum_required(VERSION 3.14)

project(doodle-lib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYTHON_HEADER_LOCATION "C:/Users/Denis/AppData/Local/Programs/Python/Python311/include")
set(PYTHON_LIB_LOCATION "C:/Users/Denis/AppData/Local/Programs/Python/Python311/libs")
set(PYBIND11_PYTHON_VERSION 3.11 CACHE STRING "")

#set(LIB_DIR ${CMAKE_SOURCE_DIR}/external)
#find_library(LIB_NAME NAMES box2d)
#link_directories(doodle-lib external/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/pybind11)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)

link_directories(${PYTHON_LIB_LOCATION})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/submodules/pybind11/include)
include_directories(${PYTHON_HEADER_LOCATION})


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

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
  pybindexample.h pybindexample.cpp
)

target_include_directories(doodle-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/include/)
target_link_libraries(doodle-lib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(doodle-lib PRIVATE box2d)

pybind11_add_module(pybindexample pybindexample.cpp)

add_compile_definitions(_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH)
target_compile_definitions(doodle-lib PRIVATE DOODLELIB_LIBRARY)
