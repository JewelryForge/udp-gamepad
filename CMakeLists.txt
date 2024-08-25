cmake_minimum_required(VERSION 3.10)
project(gamepad)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(UDP_GAMEPAD_BUILD_EXAMPLE "build example for udp-gamepad in C++" ON)

# Add your source files
add_library(gamepad_receiver SHARED
    ${CMAKE_CURRENT_LIST_DIR}/src/gamepad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/retroid_gamepad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/skydroid_gamepad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/udp_receiver.cpp
)
target_include_directories(gamepad_receiver PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

if (UDP_GAMEPAD_BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()
