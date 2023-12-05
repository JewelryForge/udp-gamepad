cmake_minimum_required(VERSION 3.10)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  option(ARM_COMPILE "Set compiler options" off)

  # Check if ARM_COMPILE is defined as an external parameter
  if(ARM_COMPILE)
      # Set the ARM compiler options
      message("this is an arm platform")
      set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
      set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
  endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gamepad)

# Add your source files
file(GLOB SOURCES  "src/*.cpp")
include_directories(
  ./include
)

# Add the fmt library as a subdirectory
add_subdirectory(fmt)
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  add_executable(gamepad_receiver ${SOURCES})
else()
  add_library(gamepad_receiver STATIC ${SOURCES})
endif()

target_link_libraries(gamepad_receiver PRIVATE fmt -lpthread -lm -lrt -ldl -lstdc++)