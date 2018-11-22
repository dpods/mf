cmake_minimum_required(VERSION 3.12)
project(mf C)

set(CMAKE_C_STANDARD 99)

add_executable(mf src/mf.c src/matrix.c src/matrix.h src/array.c src/array.h src/str.c src/str.h)

add_executable(mf2 src2/mf2.c src2/matrix.c src2/matrix.h)