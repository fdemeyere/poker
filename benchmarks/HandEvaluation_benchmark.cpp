//
// Created by legen on 2026-10-20.
//
#include "HandEvaluation.h"
#include "InputParser.h"
#include "MonteCarlo.h"
#include <benchmark/benchmark.h>

#include "benchmark/registration.h"

static void BM_monte_carlo(benchmark::State& state) {

    MCInput input {};
    input.community_cards = 0;
    input.user_cards = get_card_ext("Ad") | get_card_ext("Ac");
    input.num_players = 4;
    input.num_simulations = 1000;

    for (auto _ : state) {
        benchmark::DoNotOptimize(monte_carlo(input));
    }
}

static void BM_get_hand_sh(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize((get_card_ext("Ad")));
    }
}

BENCHMARK(BM_monte_carlo);
BENCHMARK_MAIN();