cmake_minimum_required(VERSION 3.16)
project(qdrive)

include_directories(include)

add_executable(qdrive
    src/main.c
    src/car.c
)