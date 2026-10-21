
#include "Deck.h"
#include "HandEvaluation.h"
#include <iostream>
#include <algorithm>
#include <random>

std::string get_rank_name(const Rank rank) {
    using enum Rank;

    switch (rank) {
        case two: return "two";
        case three: return "three";
        case four: return "four";
        case five: return "five";
        case six: return "six";
        case seven: return "seven";
        case eight: return "eight";
        case nine: return "nine";
        case ten: return "ten";
        case jack: return "jack";
        case queen: return "queen";
        case king: return "king";
        case ace: return "ace";
        default: return "none";
    }
}

std::string get_suit_name(const Suit suit) {
    using enum Suit;

    switch (suit) {
        case spades: return "spades";
        case hearts: return "hearts";
        case clubs: return "clubs";
        case diamonds: return "diamonds";
        default: return "none";
    }
}

std::ostream& operator<< (std::ostream& out, const Rank rank) {
    out << get_rank_name(rank);
    return out;
}

std::ostream& operator<< (std::ostream& out, const Suit suit) {
    out << get_suit_name(suit);
    return out;
}

std::ostream& operator<< (std::ostream& out, const Card& card) {
    out << "(" << card.rank << ", " << card.suit << ")";
    return out;
}

bool operator== (const Card& card1, const Card& card2) {
    return card1.rank == card2.rank && card1.suit == card2.suit;
}

bool operator< (const Card& card1, const Card& card2) {
    return card1.rank < card2.rank;
}

bool operator> (const Card& card1, const Card& card2) {
    return card1.rank > card2.rank;
}

bool operator>=(const Card& card1, const Card& card2) {
    return card1.rank >= card2.rank;
}

bool operator<=(const Card& card1, const Card& card2) {
    return card1.rank <= card2.rank;
}

bool operator!=(const Card& card1, const Card& card2) {
    return card1.rank != card2.rank;
}

Deck::Deck(uint64_t starting_cards) : m_cards {}, m_dead_cards {}, m_size {}, m_starting_size {}, m_gen {std::random_device{}()} {
    for (int i {}; i < 4; ++i) {
        for (int j {2}; j < 15; ++j) {
            uint64_t card = 1ULL << (i*16 + j);
            if (starting_cards & card)
                m_cards[m_starting_size++] = card;
        }
    }
    m_size = m_starting_size;
}

uint64_t Deck::pop(int num) {
    uint64_t res {};

    for (int i {}; i < num; ++i) {
        std::uniform_int_distribution<> distrib(0, m_size - 1);
        int index {distrib(m_gen)};
        while (m_dead_cards & m_cards[index]) {
            index = distrib(m_gen);
        }
        const uint64_t card {m_cards[index]};
        std::swap(m_cards[index], m_cards[--m_size]);

        res |= card;
    }
    return res;
}

void Deck::reset() {
    m_size = m_starting_size;
    m_dead_cards = 0ULL;
}

int Deck::size() const {
    return m_size;
}

void Deck::remove(uint64_t cards) {
    m_dead_cards |= cards;
}
