cmake_minimum_required(VERSION 3.22)
project(Matrix_Multiplication C)

set(CMAKE_C_STANDARD 23)

add_executable(Matrix_Multiplication main.c util.h string-operations.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(Matrix_Multiplication PRIVATE Threads::Threads)