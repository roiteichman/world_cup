cmake_minimum_required(VERSION 3.21)
project(world_cup)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(__MACOSX)

add_executable(world_cup
        main.cpp
        player.h
)
