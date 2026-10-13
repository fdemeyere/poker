cmake_minimum_required(VERSION 4.0)
project(poker)

set(CMAKE_CXX_STANDARD 20)

add_executable(poker main.cpp
        Deck.cpp
        Deck.h
)
