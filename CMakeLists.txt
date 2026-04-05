cmake_minimum_required(VERSION 3.29)
project(etude C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)

add_executable(etude
        src/main.c
        src/file_manager.c
        src/display.c
        src/input_handler.c
        src/config.c
        src/config.h
        src/display.h
        src/file_manager.h
        src/input_handler.h
)

target_include_directories(etude PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(etude PRIVATE ${CURSES_LIBRARIES})
