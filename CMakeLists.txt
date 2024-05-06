cmake_minimum_required(VERSION 3.28)
project(glx C)

set(CMAKE_C_STANDARD 11)


# lib-raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

# Define PROJECT_SOURCES as a list of all source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_compile_options(-Wall -Wextra -Wswitch-enum -pedantic -std=c11)
add_executable(${PROJECT_NAME}
        src/glx.c
        src/text.h
        src/text.c
)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
