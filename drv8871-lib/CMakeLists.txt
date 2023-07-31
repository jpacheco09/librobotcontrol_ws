cmake_minimum_required(3.10)

project(DRV8871 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)    
add_library(DRV8871 SHARED 
            src/DRV8871.cpp
            )
install(TARGETS DRV8871 DESTINATION /usr/lib)
install(FILES include/DRV8871.hpp DESTINATION include)    

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

