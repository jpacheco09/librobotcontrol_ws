cmake_minimum_required(VERSION 3.10)
project(pinmux_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(pinmux_test src/main.cpp)
target_link_libraries(pinmux_test robotcontrol HardwareBeagleBoneBlue)
