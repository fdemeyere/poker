cmake_minimum_required(VERSION 4.0)
project(poker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b66cffcd5f5d6ee34a4f96d57bf016d9a22cb023.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(poker main.cpp
        Deck.cpp
        Deck.h
        HandEvaluation.cpp
        HandEvaluation.h
)

add_executable(HandEvaluation_test
        HandEvaluation_test.cpp
        Deck.cpp
        Deck.h
        HandEvaluation.cpp
        HandEvaluation.h
)

target_link_libraries(
        HandEvaluation_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(HandEvaluation_test)