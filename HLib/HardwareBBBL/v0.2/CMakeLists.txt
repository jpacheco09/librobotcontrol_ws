cmake_minimum_required(VERSION 3.10)

project(HARDWARE_BEAGLEBONE_BLUE VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#find_library(robotcontrol REQUIRED)

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(BBBLLogger SHARED 
            src/Logger.cpp
            )
install(TARGETS BBBLLogger DESTINATION /usr/lib)
install(FILES include/Logger.hpp DESTINATION include)            

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(HardwareBeagleBoneBlue SHARED 
            src/HardwareBeagleBoneBlue.cpp)
            
install(TARGETS HardwareBeagleBoneBlue DESTINATION /usr/lib)
install(FILES include/HardwareBeagleBoneBlue.hpp DESTINATION include)
            

# we don't need to tinker with the path to run the executable
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

#add_subdirectory(lib)
#add_subdirectory(src)

