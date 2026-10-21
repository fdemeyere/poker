#include "Deck.h"
#include "HandEvaluation.h"
#include "MonteCarlo.h"
#include "Constants.h"
#include <random>
#include <vector>
#include <iostream>


MCResult monte_carlo(MCInput& input) {
    MCResult res {0, 0, 0};

    std::array<uint64_t, Config::max_player_count> pockets {input.user_cards};
    uint64_t community_cards {};

    const uint64_t in_use_cards {input.user_cards | input.community_cards};
    Deck deck {0x7FFC7FFC7FFC7FFCULL ^ in_use_cards};

    std::vector<uint64_t> valid_opp_pockets {};
    for (uint64_t cards : input.opponent_cards_range) {
        if ((in_use_cards & cards) == 0) {
            valid_opp_pockets.push_back(cards);
        }
    }

    std::random_device rd {};
    std::mt19937 gen {rd()};

    for (int i {}; i < input.num_simulations; ++i) {
        deck.reset();
        community_cards = input.community_cards;

        if (!input.opponent_cards_range.empty()) {
            size_t remaining_pockets = valid_opp_pockets.size();

            uint64_t dealt_opp_pocket_cards {};
            for (int player_idx {1}; player_idx < input.num_players; ) {

                if (remaining_pockets == 0) {
                    std::cout << "Too narrow opponent card range";
                    exit(0);
                }

                std::uniform_int_distribution<size_t > distrib(0, --remaining_pockets);
                const size_t idx = distrib(gen);
                std::swap(valid_opp_pockets[idx], valid_opp_pockets[remaining_pockets]);
                const uint64_t combo = valid_opp_pockets[remaining_pockets];

                if (!(dealt_opp_pocket_cards & combo)) {
                    pockets[player_idx++] = combo;
                    dealt_opp_pocket_cards |= combo;
                }
            }
            deck.remove(dealt_opp_pocket_cards);
        }
        else {
            for (int j {1}; j < input.num_players; ++j) {
                pockets[j] = deck.pop(2);
            }
        }

        community_cards |= deck.pop(5 - std::popcount(input.community_cards));

        const int game_result = user_is_winner(std::span(pockets), community_cards, input.num_players);
        if (game_result == 1) ++res.wins;
        else if (game_result == -1) ++res.losses;
        else ++res.ties;
    }
    return res;
}