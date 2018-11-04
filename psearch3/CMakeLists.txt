cmake_minimum_required(VERSION 3.12)
project(psearch3 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(psearch3 main.c)