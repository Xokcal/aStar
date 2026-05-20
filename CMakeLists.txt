cmake_minimum_required(VERSION 3.27)
project(AStar C)

set(CMAKE_C_STANDARD 11)

add_executable(
        AStar aStart/main.c
        aStart/aStar.c
        aStart/aStar.h
        aStart/typedef.h
)
