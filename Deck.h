//
// Created by legen on 2026-10-12.
//

#ifndef POKER_DECK_H
#define POKER_DECK_H

#include <vector>
#include <string_view>
#include <random>
#include <ostream>
#include <optional>
#include <array>
#include <cstddef> // for std::size_t

enum class Rank {
    two = 2,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace,
};

enum class Suit {
    spades = 0,
    hearts,
    clubs,
    diamonds,
};

inline constexpr std::array ranks {
    Rank::two, Rank::three, Rank::four, Rank::five, Rank::six, Rank::seven,
    Rank::eight,Rank::nine, Rank::ten, Rank::jack, Rank::queen, Rank::king,
    Rank::ace
};

inline constexpr std::array suits {
    Suit::spades, Suit::hearts, Suit::clubs, Suit::diamonds
};

struct Card {
    Card(Suit suit, Rank rank);
    Rank rank; // 2,3,4,5,6,7,8,9,10,11,12,13,14
    Suit suit; // 0,1,2,3

    Card(const std::string &card_sh);
    Card() = default;
};

std::string get_rank_name(Rank rank);
std::string get_suit_name(Suit suit);

std::ostream& operator<< (std::ostream& out, Rank rank);
std::ostream& operator<< (std::ostream& out, Suit suit);
std::ostream& operator<< (std::ostream& out, const Card& card);

bool operator== (const Card& card1, const Card& card2);

class Deck {
public:
    Deck(uint64_t starting_cards);
    uint64_t pop(int num);
    void reset();
    int size() const;
private:
    std::array<uint64_t, 52> m_cards;
    std::array<uint64_t, 52> m_starting_cards;
    int m_size;
    int m_starting_size;
    std::mt19937 m_gen;
};


#endif //POKER_DECK_H