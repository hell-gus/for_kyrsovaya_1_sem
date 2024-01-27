cmake_minimum_required(VERSION 3.27)
project(untitled2 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled2 main.c
        structs.h
        libraries.h
        input_and_output.h
        input_and_output.c
        funcs.h
        funcs.c
        first_formating.h
        first_formating.c)
