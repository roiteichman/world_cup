cmake_minimum_required(VERSION 3.21)
project(world_cup)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(__MACOSX)

add_executable(world_cup
        __MACOSX/._main23a1.cpp
        __MACOSX/._wet1util.h
        __MACOSX/._worldcup23a1.cpp
        __MACOSX/._worldcup23a1.h
        main23a1.cpp
        player.h
        team.h
        team.cpp
        wet1util.h
        worldcup23a1.cpp
        worldcup23a1.h)
