//
// Created by legen on 2026-10-13.
//

#ifndef POKER_HANDEVALUATOR_H
#define POKER_HANDEVALUATOR_H

#include "Deck.h"
#include <ostream>
#include <cinttypes>

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

uint64_t get_cards_ext(const std::array<Card, 7>& cards);

std::array<Rank, 5> get_ranks_from_hand_sh(uint32_t hand);

uint16_t spades(uint64_t mask);

uint16_t hearts(uint64_t mask);

uint16_t clubs(uint64_t mask);

uint16_t diamonds(uint64_t mask);

uint32_t get_hand_sh(const std::array<Card, 7>& cards);

void print_hand_ext(uint64_t mask);
void print_hand_sh(uint32_t mask);



#endif //POKER_HANDEVALUATOR_H