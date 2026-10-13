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
    two,
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
    spades,
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
    Rank rank;
    Suit suit;
};

std::string_view get_rank_name(const Rank rank);
std::string_view get_suit_name(const Suit suit);

std::ostream& operator<< (std::ostream& out, const Rank rank);
std::ostream& operator<< (std::ostream& out, const Suit suit);
std::ostream& operator<< (std::ostream& out, const Card& card);

bool operator== (const Card& card1, const Card& card2);

class Deck {
public:
    Deck();
    const std::vector<Card>& get_deck() const {
        return m_deck;
    }
    std::optional<Card> pop_card();
    void shuffle();
    bool is_empty() const;
    std::size_t size() const;
private:
    std::vector<Card> m_deck;
    std::mt19937 m_gen;
};


#endif //POKER_DECK_H