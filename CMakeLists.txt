cmake_minimum_required(VERSION 4.1)
project(pong_game)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(pong_game main.cpp
        Game.h
        Game.cpp)

target_link_libraries(pong_game PRIVATE SDL2::SDL2)