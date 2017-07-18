cmake_minimum_required(VERSION 3.0)

project(hatchery)

set(CMAKE_C_FLAGS "-O2 -ldl -lm")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

include_directories(../common)
include_directories(../log)
include_directories(../../lua)

link_directories(../../lua)
link_directories(../log)

aux_source_directory(. DIR_SRCS)

add_executable(hatchery hatchery.c)

target_link_libraries(hatchery lua LogFunctions )

