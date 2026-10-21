
#ifndef POKER_MONTECARLO_H
#define POKER_MONTECARLO_H

#include <cstdint>
#include <vector>

struct MCInput {
    uint64_t community_cards;
    uint64_t user_cards;
    int num_players;
    std::vector<uint64_t> opponent_cards_range;
    int num_simulations {100000};
};

struct MCResult {
    int wins;
    int ties;
    int losses;
};

MCResult monte_carlo(MCInput& input);

#endif //POKER_MONTECARLO_H