cmake_minimum_required(VERSION 3.28)
project(GameLex C)

set(CMAKE_C_STANDARD 11)

set(GLX_COMMON_SOURCES ./src/text.c ./src/glex.c
        src/shift.h
        src/shift.c)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(GLX_COMMON_LIB ${GLX_COMMON_SOURCES})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wswitch-enum -pedantic -std=c11 -ggdb")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(gla)
add_executable(gle)

target_sources(gla PRIVATE ./src/glc.c ./src/assembler.c)
target_sources(gle PRIVATE ./src/gle.c ./src/emulator.c)

target_link_libraries(gla PRIVATE GLX_COMMON_LIB)
target_link_libraries(gle PRIVATE GLX_COMMON_LIB)
