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

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
)
FetchContent_MakeAvailable(googlebenchmark)

enable_testing()

add_executable(poker src/main.cpp
        src/Deck.cpp
        src/HandEvaluation.cpp
        src/MonteCarlo.cpp
        src/InputParser.cpp
)

target_include_directories(poker PUBLIC include)

add_executable(HandEvaluation_test
        tests/HandEvaluation_test.cpp
        src/Deck.cpp
        src/HandEvaluation.cpp
)

target_include_directories(HandEvaluation_test PUBLIC include)

target_link_libraries(
        HandEvaluation_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(HandEvaluation_test)

add_executable(HandEvaluation_benchmark
        benchmarks/HandEvaluation_benchmark.cpp
        src/Deck.cpp
        src/HandEvaluation.cpp
        benchmarks/HandEvaluation_benchmark.cpp
        src/MonteCarlo.cpp
        src/InputParser.cpp
)

target_include_directories(HandEvaluation_benchmark PUBLIC include)

target_link_libraries(
        HandEvaluation_benchmark
        PRIVATE
        benchmark::benchmark_main
)