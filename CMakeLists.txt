cmake_minimum_required(VERSION 3.29)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled
        main.c
        src/file_manager.c
        src/display.c
        src/input_handler.c
        src/config.c
        src/config.h
        src/display.h
        src/file_manager.h
        src/input_handler.h
)
