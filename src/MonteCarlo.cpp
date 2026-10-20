#include "Deck.h"
#include "HandEvaluation.h"
#include "MonteCarlo.h"

//
// Created by legen on 2026-10-20.
//


uint64_t monte_carlo(const MCInput& input) {

    // res: win/tie/loss counts stored by value
    // [63-42: wins][41-21: ties][20-0: losses]
    uint64_t res {};
    uint64_t inter_result {};

    Deck deck {0x7FFC7FFC7FFC7FFCULL ^ (input.user_cards | input.community_cards)};
    std::array<uint64_t, 23> pockets {input.user_cards};
    uint64_t community_cards {};
    for (int i {}; i < input.num_simulations; ++i) {
        deck.reset();
        community_cards = input.community_cards;

        for (int j {1}; j < input.num_players; ++j) {
            pockets[j] = deck.pop(2);
        }
        for (int j {}; j < 5 - std::popcount(input.community_cards); ++j) {
            community_cards |= deck.pop(1);
        }

        inter_result = user_is_winner(std::span(pockets), community_cards, input.num_players);
        if (inter_result == 1) {
            res += 1ULL << 42;
        }
        else if (inter_result == -1) {
            res += 1ULL;
        }
        else res += 1ULL << 21;
    }
    return res;
}