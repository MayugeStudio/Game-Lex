cmake_minimum_required(VERSION 3.28)
project(GameLex C)

set(CMAKE_C_STANDARD 11)

set(GLX_COMMON_SOURCES ./src/text.c ./src/glex.c)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(GLX_COMMON_LIB ${GLX_COMMON_SOURCES})

# toolchain names
# set(GLX_COMPILER glc)       # Game-Lex-Compiler
set(GLX_ASSEMBLER glasm)      # Game-Lex-Assembler
set(GLX_EMULATOR gle)         # Game-Lex-ASM-Emulator

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wswitch-enum -pedantic -std=c11 -ggdb")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(GLX_ASSEMBLER)
add_executable(GLX_EMULATOR)

target_sources(GLX_ASSEMBLER PRIVATE ./src/glc.c ./src/assembler.c)
target_sources(GLX_EMULATOR PRIVATE ./src/gle.c ./src/emulator.c)

target_link_libraries(GLX_ASSEMBLER PUBLIC GLX_COMMON_LIB)
target_link_libraries(GLX_EMULATOR PUBLIC GLX_COMMON_LIB)
