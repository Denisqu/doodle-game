cmake_minimum_required(VERSION 3.14)

project(ml-lib LANGUAGES CXX)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYTHON_HEADER_LOCATION "C:/Users/Denis/AppData/Local/Programs/Python/Python311/include")
set(PYTHON_LIB_LOCATION "C:/Users/Denis/AppData/Local/Programs/Python/Python311/libs")
set(PYBIND11_PYTHON_VERSION 3.11 CACHE STRING "")

#find_package(Torch REQUIRED)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network WebSockets)

# set_property(TARGET ml-lib PROPERTY CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/pybind11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/submodules/pybind11/include)
include_directories(${PYTHON_HEADER_LOCATION})
link_directories(${PYTHON_LIB_LOCATION})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../doodle-lib/external)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/jcon-cpp/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/submodules/jcon-cpp/src/jcon)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/jcon-cpp)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/jcon-cpp/include)

#libs\doodle-lib

set(CMAKE_AUTOMOC ON)

add_library(ml-lib SHARED
  ml-lib_global.h
  mllib.cpp
  mllib.h
  agent.h agent.cpp
  dqn.h dqn.cpp
  doodleenv.h doodleenv.cpp
  mlview.h mlview.cpp
  mlserver.h mlserver.cpp
  mlclient.h mlclient.cpp
  pybindexample.h pybindexample.cpp
  exampleservice.h exampleservice.cpp
)

pybind11_add_module(pybindexample pybindexample.cpp)
target_include_directories(ml-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../doodle-lib/external/include/)
target_link_libraries(ml-lib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::WebSockets)
target_link_libraries(ml-lib PRIVATE doodle-lib box2d jcon)


# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
#if (MSVC)
#  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
#  add_custom_command(TARGET ml-lib
#                     POST_BUILD
#                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                     ${TORCH_DLLS}
#                     $<TARGET_FILE_DIR:ml-lib>)
#endif (MSVC)

#target_link_libraries(ml-lib PRIVATE "${TORCH_LIBRARIES}")
target_compile_definitions(ml-lib PRIVATE MLLIB_LIBRARY)
