cmake_minimum_required(VERSION 3.10)

project(libtest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(i2c_test src/test_i2c.cpp)
target_link_libraries(i2c_test robotcontrol HardwareBeagleBoneBlue)

add_executable(eqep_test src/eqep_test.cpp)
target_link_libraries(eqep_test robotcontrol HardwareBeagleBoneBlue)
#add_executable(pca9685 src/pwm_test.cpp)
#target_link_libraries(pca9685 robotcontrol HardwareBeagleBoneBlue)

add_executable(curr_cal src/ina260_cal.cpp)
target_link_libraries(curr_cal robotcontrol HardwareBeagleBoneBlue)
