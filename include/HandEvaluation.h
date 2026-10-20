//
// Created by legen on 2026-10-13.
//

#ifndef POKER_HANDEVALUATOR_H
#define POKER_HANDEVALUATOR_H

#include "Deck.h"
#include <ostream>
#include <cinttypes>
#include <span>

enum class HandType {
    high_card = 0,
    one_pair,
    two_pair,
    three_of_a_kind,
    straight,
    flush,
    full_house,
    four_of_a_kind,
    straight_flush,
    royal_flush = 9,
};

std::ostream& operator<<(std::ostream& out, HandType type);

std::string_view get_hand_type_str(HandType type);

HandType get_type_from_hand_sh(uint32_t hand);

std::array<Rank, 5> get_ranks_from_hand_sh(uint32_t hand);

uint32_t get_hand_sh(uint64_t cards);
int user_is_winner(const std::span<uint64_t, 23>& pockets, uint64_t community_cards, int num_players);

void print_hand_sh(uint32_t mask);

uint64_t get_card_ext(const std::string &card_str);
#endif //POKER_HANDEVALUATOR_H