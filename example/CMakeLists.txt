cmake_minimum_required(VERSION 3.10)
project(gamepad_example)


include_directories(
    fmt/include
)
add_subdirectory(fmt)


add_executable(example_skydroid example_skydroid.cpp)
target_link_libraries(example_skydroid udp_gamepad fmt)

add_executable(example_retroid example_retroid.cpp)
target_link_libraries(example_retroid udp_gamepad fmt)