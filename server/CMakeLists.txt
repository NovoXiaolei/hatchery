cmake_minimum_required(VERSION 3.0)

project(hatchery)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -ldl -lm -pthread -std=c99")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../lib)

include_directories("${PROJECT_SOURCE_DIR}")

include_directories(../common)
include_directories(../log)
include_directories(../lua)
include_directories(../utils)

link_directories(../lua)
add_library(UtilsFuncitons STATIC ../utils/utils.c)
target_link_libraries(UtilsFuncitons lua)

add_library(LogFunctions STATIC ../log/log.c)

aux_source_directory(. DIR_SRCS)

add_executable(hatchery hatchery.c)

target_link_libraries(hatchery lua LogFunctions UtilsFuncitons)

