cmake_minimum_required(VERSION 3.12)

project(${EXEC_NAME}
    VERSION 0.1
    DESCRIPTION "A WinAPI hello world"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)

# Also look for Find-cmake modules here
#set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/sdl2")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c11 -D_DEBUG=1 -Wall -Wextra -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c11 -O3 -D_DEBUG=0")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc")


set(SOURCES
    src/main.c
)

set(INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/src"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
