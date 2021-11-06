cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include)

project(project_6502)

file(GLOB_RECURSE SRCS src/*.c)

add_library(6502 SHARED ${SRCS})

target_link_libraries(6502 m)
if(NOT WIN32)
    target_link_libraries(6502 dl)
endif(NOT WIN32)