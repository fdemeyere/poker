
#include "HandEvaluation.h"
#include "MonteCarlo.h"
#include <benchmark/benchmark.h>

#include "benchmark/registration.h"

static void BM_monte_carlo(benchmark::State& state) {

    MCInput input {};
    input.community_cards = 0;
    input.user_cards = *get_card_ext("Ad") | *get_card_ext("Ac");
    input.num_players = 4;
    input.num_simulations = 100000;

    for (auto _ : state) {
        benchmark::DoNotOptimize(monte_carlo(input));
    }
}

static void BM_get_hand_sh(benchmark::State& state) {
    const uint64_t high_card {*get_card_ext("Ad") | *get_card_ext("2c") |
    *get_card_ext("5d") | *get_card_ext("Ts") | *get_card_ext("Jh") |
        *get_card_ext("4c") | *get_card_ext("Qs")};

    const uint64_t full_house {*get_card_ext("Ad") | *get_card_ext("Ac") |
    *get_card_ext("As") | *get_card_ext("Ts") | *get_card_ext("Th") |
        *get_card_ext("4c") | *get_card_ext("Qs")};

    for (auto _ : state) {
        benchmark::DoNotOptimize(get_hand_sh(full_house));
    }
}

BENCHMARK(BM_get_hand_sh);
BENCHMARK_MAIN();