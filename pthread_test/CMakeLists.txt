cmake_minimum_required(VERSION 3.10)
project(phtread_local)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_library(ROBOTCONTROL_LIB liblibrobotcontrol.so)
message(STATUS ${ROBOTCONTROL_LIB})

find_library(HARDWAREBEAGLEBONEBLUE_LIB libHardwareBeagleBoneBlue.so)
message(STATUS ${HARDWAREBEAGLEBONEBLUE_LIB})

#add_executable(phtread_test src/main.cpp)
#target_link_libraries(phtread_test PRIVATE ${HARDWAREBEAGLEBONEBLUE_LIB} ${ROBOTCONTROL_LIB})

add_executable(slip src/sleep_thread.cpp)
