cmake_minimum_required(VERSION 3.12)
project(mf C)

set(CMAKE_C_STANDARD 99)

add_executable(mf src/mf.c src/matrix.c src/matrix.h)