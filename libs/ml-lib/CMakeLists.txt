cmake_minimum_required(VERSION 3.14)

project(ml-lib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


# set_property(TARGET ml-lib PROPERTY CXX_STANDARD 17)



add_library(ml-lib SHARED
  ml-lib_global.h
  mllib.cpp
  mllib.h
  agent.h agent.cpp
  dqn.h dqn.cpp
)

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET ml-lib
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:ml-lib>)
endif (MSVC)

target_link_libraries(ml-lib PRIVATE "${TORCH_LIBRARIES}")
target_compile_definitions(ml-lib PRIVATE MLLIB_LIBRARY)
