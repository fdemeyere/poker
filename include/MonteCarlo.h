//
// Created by legen on 2026-10-20.
//

#ifndef POKER_MONTECARLO_H
#define POKER_MONTECARLO_H

#include <cstdint>

struct MCInput {
    uint64_t community_cards;
    uint64_t user_cards;
    int num_players;
    int num_simulations {1000};
    uint64_t opponent_cards_range;
};

uint64_t monte_carlo(const MCInput& input);

#endif //POKER_MONTECARLO_H