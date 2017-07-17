cmake_minimum_required(VERSION 3.0)

project (hatchery)

set(CMAKE_C_FLAGS "-O2 -ldl -lm")

include_directories(../common)
include_directories(../log)
include_directories(../../lua)

link_directories(../../lua)
link_directories(../log)

aux_source_directory(. DIR_SRCS)

add_executable(hatchery hatchery.c)

target_link_libraries(hatchery lua LogFunctions )

