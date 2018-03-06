cmake_minimum_required(VERSION 3.9)
project(task02 C)

set(CMAKE_C_STANDARD 99)

add_executable(task02 main.c stack.h queue.h input_parser.c input_parser.h vector.h vector.c expression_parser.c expression_parser.h calculator.c calculator.h)