cmake_minimum_required(VERSION 3.14)

project(ml-lib-tests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(gtest_force_shared_crt on)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED TEST)

add_subdirectory(external/googletest-1.14.0)

add_executable(ml-lib-tests
    #mllibtest.cpp
    mlservertest.cpp
    mlserverwebsocketclienttests.cpp
    websockettestclient.h #websockettestclient.cpp
)
target_link_libraries(ml-lib-tests PUBLIC
    gtest_main
    ml-lib
)
target_link_libraries(ml-lib-tests
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_libraries(ml-lib-tests PRIVATE doodle-lib ml-lib)

# For Windows: Prevent overriding the parent project's compiler/linker settings


add_test(
    NAME ml-lib-tests
    COMMAND ml-lib-tests
)
