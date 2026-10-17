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

namespace {
    uint16_t hearts(const uint64_t mask) {
        return (mask >> 16) & 0xFFFF;
    }

    uint16_t clubs(const uint64_t mask){
        return (mask >> 32) & 0xFFFF;
    }

    uint16_t diamonds(const uint64_t mask){
        return (mask >> 48) & 0xFFFF;
    }

    uint16_t spades(const uint64_t mask) {
        return mask & 0xFFFF;
    }

    void print_hand_ext(const uint64_t mask) {
        std::cout << "Diamonds: " << std::bitset<16>(diamonds(mask)) << "\n"
                  << "Clubs:    " << std::bitset<16>(clubs(mask)) << "\n"
                  << "Hearts:   " << std::bitset<16>(hearts(mask)) << "\n"
                  << "Spades:   " << std::bitset<16>(spades(mask)) << "\n";
    }

    // hand_ext format: ranks stored by index
    // [64-48 = diamonds] [47-32 = clubs] [31-16 = hearts] [15-0 = spades]
    uint64_t get_cards_ext(const std::array<Card, 7>& cards) {
        uint64_t mask {};
        for (const Card& card : cards) {
            mask |= 1ULL << (static_cast<int>(card.suit) * 16U + static_cast<int>(card.rank));
        }
        return mask;
    }
}

std::string_view get_hand_type_str(const HandType type) {
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
        default: return "none";
    }
}

std::ostream& operator<<(std::ostream& out, const HandType type) {
    out << get_hand_type_str(type);
    return out;
}

// Print a 32 bit hand representation, no suit info
void print_hand_sh(const uint32_t mask) {
    std::cout << "32 bit hand repr: " << std::bitset<32>(mask) << "\n";
}

// function taking in a 32 bit hand representation and returning just the HandType
HandType get_type_from_hand_sh(const uint32_t hand) {
    return static_cast<HandType>(hand >> 20);
}

std::array<Rank, 5> get_ranks_from_hand_sh(const uint32_t hand) {
    std::array<Rank, 5> res{};
    res[0] = static_cast<Rank>((hand >> 16) & 0xF);
    res[1] = static_cast<Rank>((hand >> 12) & 0xF);
    res[2] = static_cast<Rank>((hand >> 8) & 0xF);
    res[3] = static_cast<Rank>((hand >> 4) & 0xF);
    res[4] = static_cast<Rank>(hand & 0xF);
    return res;
}

/*  hand_sh: ranks stored by value
    [31-20 = HandType 0-9][19-16 = 1st rank][15-12 = 2nd rank][11-8 = 3rd rank][7-4 = 4th rank][3-0 = 5th rank]
*/
uint32_t get_hand_sh(const std::array<Card, 7>& cards) {

    const uint64_t mask64 = get_cards_ext(cards);

    const uint16_t s {spades(mask64)};
    const uint16_t h {hearts(mask64)};
    const uint16_t c {clubs(mask64)};
    const uint16_t d {diamonds(mask64)};

    const std::array suit_masks {s, h, c, d};

    const uint16_t at_least_2 = (s & h) | (s & c) | (s & d) | (h & c) | (h & d) | (c & d);
    const uint16_t at_least_3 = (s & h & c) | (s & h & d) | (s & c & d) | (h & c & d);
    const uint16_t quad (s & h & c & d);

    const uint16_t pairs = at_least_2 ^ at_least_3;
    const uint16_t trips = at_least_3 ^ quad;

    const uint16_t all_ranks = s | h | c | d;
    const uint16_t all_ranks_ace_low = all_ranks | (all_ranks >> 13);

    const uint16_t straights = all_ranks_ace_low & (all_ranks_ace_low << 1)
        & (all_ranks_ace_low << 2) & (all_ranks_ace_low << 3) & (all_ranks_ace_low << 4);


    // royal flush, straight flush, flush
    for (const uint16_t suit : suit_masks) {
        if (std::popcount(suit) < 5) {
            continue;
        }

        // royal flush
        if ((suit & (0b11111U << 10)) == (0b11111U << 10)) {
            uint32_t res {9U << 20};
            for (int j {}; j < 5; ++j) {
                res |= ((14U - j) << (16 - 4*j));
            }
            return res;
        }

        uint16_t suit_ace_low = suit | (suit >> 13);
        const uint16_t highest_straight_flush_ranks = suit_ace_low & (suit_ace_low << 1)
            & (suit_ace_low << 2) & (suit_ace_low << 3) & (suit_ace_low << 4);

        // straight
        if (highest_straight_flush_ranks > 0) {
            const uint16_t highest_rank_straight_flush = std::bit_width(highest_straight_flush_ranks) - 1;
            uint32_t res {8U << 20};

            for (int j {}; j < 5; ++j) {
                // ace-low
                if (highest_rank_straight_flush == 5 && j == 4) {
                    res |= 14U;
                    break;
                }
                res |= (highest_rank_straight_flush - j) << (16 - 4*j);
            }
            return res;
        }

        // flush
        uint32_t res {5U << 20};
        uint16_t remaining {suit};

        uint16_t cur_highest_rank {};
        for (int i {}; i < 5; ++i) {
            cur_highest_rank = std::bit_width(remaining) - 1;
            res |= cur_highest_rank << (16 - 4*i);

            remaining ^= 1U << cur_highest_rank;
        }
        return res;
    }

    // four of a kind
    if (quad) {
        uint32_t res {7U << 20};
        const uint16_t quad_rank (std::bit_width(quad) - 1);
        for (int j {}; j < 4; ++j) {
            res |= (quad_rank << (16 - 4*j));
        }

        const uint16_t ranks_left = all_ranks ^ (1U << quad_rank);
        res |= static_cast<uint32_t>(std::bit_width(ranks_left) - 1);
        return res;
    }

    // full house
    if (trips && pairs) {
        uint32_t res {6U << 20};

        const uint16_t highest_trip = std::bit_width(trips) - 1;
        for (int j {}; j < 3; ++j) {
            res |= (highest_trip << (16 - 4*j));
        }

        if (std::popcount(trips) == 2) {
            uint16_t second_highest_trip = std::bit_width(trips ^ (1U << highest_trip)) - 1;
            for (int j {}; j < 2; ++j) {
                res |= (second_highest_trip << (4 - 4*j));
            }
        }

        const uint16_t pair_rank = std::bit_width(pairs) - 1;
        for (int j {}; j < 2; ++j) {
            res |= (pair_rank << (4 - 4*j));
        }

        return res;
    }

    // straight
    if (straights) {
        uint32_t res {4U << 20};
        uint16_t highest_straight = std::bit_width(straights) - 1;
        for (int i {}; i < 5; ++i) {
            if (highest_straight == 5 && i == 4) {
                res |= 14U;
                break;
            }
            res |= static_cast<uint32_t>((highest_straight - i) << (16 - 4*i));
        }

        return res;
    }

    // three of a kind
    if (trips) {
        uint32_t res {3U << 20};
        const uint16_t highest_trip = std::bit_width(trips) - 1;
        res |= highest_trip << 16;
        res |= highest_trip << 12;
        res |= highest_trip << 8;
        uint16_t remaining_ranks = all_ranks ^ (1U << highest_trip);

        uint16_t highest_non_trip = std::bit_width(remaining_ranks) - 1;
        res |= highest_non_trip << 4;

        remaining_ranks ^= 1U << highest_non_trip;
        highest_non_trip = std::bit_width(remaining_ranks) - 1;
        res |= highest_non_trip;
        return res;
    }

    // two pair
    if (std::popcount(pairs) >= 2) {
        uint32_t res {2U << 20};
        uint16_t highest_pair = std::bit_width(pairs) - 1;
        res |= highest_pair << 16;
        res |= highest_pair << 12;

        uint16_t second_highest_pair = std::bit_width(pairs ^ (1U << highest_pair)) - 1;
        res |= second_highest_pair << 8;
        res |= second_highest_pair << 4;

        uint16_t kicker = all_ranks ^ (1U << highest_pair) ^ (1U << second_highest_pair);

        res |= static_cast<uint32_t>(std::bit_width(kicker) - 1);

        return res;
    }

    // one pair
    if (pairs) {
        uint32_t res {1U << 20};

        uint16_t pair_rank = std::bit_width(pairs) - 1;
        res |= pair_rank << 16;
        res |= pair_rank << 12;

        uint16_t remaining_ranks = all_ranks ^ (1U << pair_rank);
        for (int i {}; i < 3; ++i) {
            uint16_t highest_rank = std::bit_width(remaining_ranks) - 1;
            res |= highest_rank << (8 - 4*i);
            remaining_ranks ^= (1U << highest_rank);
        }
        return res;
    }

    // high card
    uint32_t res {};
    uint16_t remaining_ranks = all_ranks;
    for (int i {}; i < 5; ++i) {
        uint16_t highest_rank = std::bit_width(remaining_ranks) - 1;
        res |= highest_rank << (16 - 4*i);
        remaining_ranks ^= (1U << highest_rank);
    }
    return res;
}
