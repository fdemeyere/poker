// //
// // Created by legen on 2026-10-13.
// //
//
#include "HandEvaluation.h"
#include "Deck.h"
#include <iostream>
#include <string_view>
#include <bit>
#include <bitset>

std::string_view get_hand_type_str(HandType type) {
    using enum HandType;

    switch (type) {
        case high_card: return "High card";
        case one_pair: return "One pair";
        case two_pair: return "Two pair";
        case three_of_a_kind: return "three_of_a_kind";
        case straight: return "straight";
        case flush: return "flush";
        case full_house: return "full_house";
        case four_of_a_kind: return "four_of_a_kind";
        case straight_flush: return "straight_flush";
        case royal_flush: return "royal_flush";
        default: return "???";
    }
}

std::ostream& operator<<(std::ostream& out, const HandType type) {
    out << get_hand_type_str(type);
    return out;
}

void print_hand_ext(uint64_t mask) {
    std::cout << "Diamonds: " << std::bitset<16>(diamonds(mask)) << "\n"
              << "Clubs:    " << std::bitset<16>(clubs(mask)) << "\n"
              << "Hearts:   " << std::bitset<16>(hearts(mask)) << "\n"
              << "Spades:   " << std::bitset<16>(spades(mask)) << "\n";
}

// Print a 32 bit hand representation, no suit info
void print_hand_sh(uint32_t mask) {
    std::cout << "32 bit hand repr: " << std::bitset<32>(mask) << "\n";
}

// function taking in a 32 bit hand representation and returning just the HandType
HandType get_type_from_hand_sh(uint32_t hand) {
    return static_cast<HandType>(hand >> 20);
}

uint64_t get_cards_ext(const std::array<Card, 7>& cards) {
    uint64_t mask {};
    for (const Card& card : cards) {
        mask |= 1ULL << (static_cast<int>(card.suit) * 16 + static_cast<int>(card.rank));
    }
    return mask;
}

uint16_t spades(uint64_t mask) {
    return mask & 0xFFFF;
}

uint16_t hearts(uint64_t mask) {
    return (mask >> 16) & 0xFFFF;
}

uint16_t clubs(uint64_t mask){
    return (mask >> 32) & 0xFFFF;
}

uint16_t diamonds(uint64_t mask){
    return (mask >> 48) & 0xFFFF;
}

bool get_highest_straights(uint16_t mask) {
    return mask & (mask << 1) & (mask << 2) & (mask << 3) & (mask << 4);
}

std::array<Rank, 5> get_ranks_from_hand_sh(uint32_t hand) {
    std::array<Rank, 5> res{};
    res[0] = static_cast<Rank>((hand >> 16) & 0xF);
    res[1] = static_cast<Rank>((hand >> 12) & 0xF);
    res[2] = static_cast<Rank>((hand >> 8) & 0xF);
    res[3] = static_cast<Rank>((hand >> 4) & 0xF);
    res[4] = static_cast<Rank>(hand & 0xF);
    return res;
}

/* RETURN
   [31-20 = HandType 0-9][19-16 = 5th rank][15-12 = 5th rank][11-8 = 5th rank][7-4 = 4th rank][3-0 = 5th rank]
*/
uint32_t get_hand_sh(const std::array<Card, 7>& cards) {
    // [64-48 = diamonds] [47-32 = clubs] [31-16 = hearts] [15-0 = spades]
    // 2 = 2 of spades. 14 = ace of spades
    const uint64_t mask64 = get_cards_ext(cards);

    const uint16_t s {spades(mask64)};
    const uint16_t h {hearts(mask64)};
    const uint16_t c {clubs(mask64)};
    const uint16_t d {diamonds(mask64)};

    const std::array<uint16_t, 4> suit_masks {s, h, c, d};

    const uint64_t at_least_2 = (s & h) | (s & c) | (s & d) | (h & c) | (h & d) | (c & d);
    const uint64_t at_least_3 = (s & h & c) | (s & h & d) | (s & c & d) | (h & c & d);
    const uint64_t quad (s & h & c & d);

    const uint64_t pairs = at_least_2 ^ at_least_3;
    const uint64_t trips = at_least_3 ^ quad;

    const uint16_t all_ranks = s | h | c | d;

    const uint16_t straights = all_ranks & (all_ranks << 1) & (all_ranks << 2) & (all_ranks << 3) & (all_ranks << 4);

    // royal flush
    for (const uint16_t suit : suit_masks) {
        if ((suit & (0b11111 << 10)) == (0b11111 << 10)) return uint32_t {9 << 20};
    }

    // straight flush
    for (int i {}; i < suit_masks.size(); ++i) {

        // check for flush
        if (std::popcount(suit_masks[i]) >= 5) {

            // check for straight
            const uint16_t highest_straight_flush_ranks = suit_masks[i] & (suit_masks[i] << 1)
                & (suit_masks[i] << 2) & (suit_masks[i] << 3) & (suit_masks[i] << 4);

            if (highest_straight_flush_ranks == 0) {
                continue;
            }

            const uint16_t highest_rank_straight_flush = std::bit_width(highest_straight_flush_ranks) - 1;

            uint32_t res {8 << 20};

            for (uint32_t j {}; j < 5; ++j) {
                res |= ((highest_rank_straight_flush - j) << (16 - 4*j));
            }
            return res;
        }
    }

    // Four of a kind
    if (quad) {
        uint32_t res {7 << 20};
        const uint32_t quad_rank (std::bit_width(quad) - 1);
        for (uint32_t j {}; j < 4; ++j) {
            res |= (quad_rank << (16 - 4*j));
        }
        uint32_t kicker;
        const uint16_t ranks_left = all_ranks ^ (1 << quad_rank);
        res |= std::bit_width(ranks_left) - 1;
        return res;
    }

    // Full house
    if (trips && pairs) {
        uint32_t res {6 << 20};

        const uint32_t trip_rank (std::bit_width(trips) - 1);
        for (uint32_t j {}; j < 3; ++j) {
            res |= (trip_rank << (16 - 4*j));
        }

        const uint32_t pair_rank (std::bit_width(pairs) - 1);
        for (uint32_t j {}; j < 2; ++j) {
            res |= (pair_rank << (4 - 4*j));
        }

        return res;
    }

    // Flush
    for (const uint16_t suit : suit_masks) {
        if (std::popcount(suit) >= 5) {
            uint32_t res {5 << 20};
            uint16_t remaining {suit};

            uint16_t cur_highest_rank {};
            for (uint32_t i {}; i < 5; ++i) {
                cur_highest_rank = std::bit_width(remaining) - 1;
                res |= (cur_highest_rank << (16 - 4*i));

                remaining ^= (1 << cur_highest_rank);
            }
            return res;
        }
    }

    // Straight
    if (straights) {
        uint32_t res {4 << 20};
        uint16_t highest_straight = std::bit_width(straights) - 1;
        for (uint32_t i {}; i < 5; ++i) {
            res |= (highest_straight - i) << (16 - 4*i);
        }

        return res;
    }

    // Three of a kind
    if (trips) {
        uint32_t res {3 << 20};
        const uint16_t highest_trip = std::bit_width(trips) - 1;
        res |= highest_trip << 16;
        res |= highest_trip << 12;
        res |= highest_trip << 8;
        uint16_t remaining_ranks = all_ranks ^ (1 << highest_trip);

        uint32_t highest_non_trip (std::bit_width(remaining_ranks) - 1);
        res |= (highest_non_trip << 4);

        remaining_ranks ^= (1 << highest_non_trip);
        highest_non_trip = std::bit_width(remaining_ranks) - 1;
        res |= highest_non_trip;
        return res;
    }

    // Two pair
    if (std::popcount(pairs) >= 2) {
        uint32_t res {2 << 20};
        uint64_t highest_pair = std::bit_width(pairs) - 1;
        res |= highest_pair << 16;
        res |= highest_pair << 12;

        uint16_t second_highest_pair = std::bit_width(pairs ^ (1 << highest_pair)) - 1;
        res |= second_highest_pair << 8;
        res |= second_highest_pair << 4;

        uint16_t kicker = all_ranks ^ (1 << highest_pair) ^ (1 << second_highest_pair);

        res |= std::bit_width(kicker) - 1;

        return res;
    }

    // One pair
    if (pairs) {
        uint32_t res {1 << 20};

        uint16_t pair_rank = std::bit_width(pairs) - 1;
        res |= pair_rank << 16;
        res |= pair_rank << 12;

        uint16_t remaining_ranks = all_ranks ^ (1 << pair_rank);
        for (int i {}; i < 3; ++i) {
            uint16_t highest_rank = std::bit_width(remaining_ranks) - 1;
            res |= highest_rank << (8 - 4*i);
            remaining_ranks ^= (1 << highest_rank);
        }
        return res;
    }

    // High card
    uint32_t res {};
    uint16_t remaining_ranks = all_ranks;
    for (uint16_t i {}; i < 5; ++i) {
        uint16_t highest_rank = std::bit_width(remaining_ranks) - 1;
        res |= highest_rank << (16 - 4*i);
        remaining_ranks ^= (1 << highest_rank);
    }
    return res;
}
