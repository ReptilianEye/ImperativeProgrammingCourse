cmake_minimum_required(VERSION 3.24)
project(Nauka_C C)

set(CMAKE_C_STANDARD 11)

add_executable(Nauka_C main.c)

add_executable(fibb zad0/fibb.c)
add_executable(vectors zad1/vectors.c)
add_executable(wstepDoC wstepDoC.c)
add_executable(permutacje zad2/permutacje.c)
add_executable(struktury zad2/struktury.c)