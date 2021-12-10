cmake_minimum_required(VERSION 3.20)
project(assignment_11)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(assignment_11
        ball.cpp
        ball.h
        block.cpp
        block.h
        breakout.cpp
        breakout.h
        paddle.cpp
        paddle.h
        hellogl.cpp)
