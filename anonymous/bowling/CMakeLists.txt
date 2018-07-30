cmake_minimum_required(VERSION 3.12)
project(bowling)

set(CMAKE_CXX_STANDARD 17)

add_executable(bowling main.cpp procedural.cpp procedural.h player.cpp player.h game.cpp game.h frame.cpp frame.h roll.cpp roll.h)