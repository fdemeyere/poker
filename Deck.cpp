//
// Created by legen on 2026-10-12.
//

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

Deck::Deck() : m_deck {0ULL}, m_gen {std::random_device{}()} {
    reset();
}

uint64_t Deck::pop(const int num) {
    uint64_t res {};
    for (int i {}; i < num; ++i) {
        std::uniform_int_distribution<> distrib(1, size());
        const int count { distrib(m_gen)};
        uint64_t deck_copy {m_deck};

        for (int j {}; j < count - 1; ++j) {
            deck_copy &= deck_copy - 1;
        }

        const uint64_t card = deck_copy & ~deck_copy + 1;
        m_deck ^= card;
        res |= card;
    }
    return res;
}

void Deck::remove(const std::string& card_str) {
    const uint64_t card {get_card_ext(card_str)};
    m_deck ^= card;
}

void Deck::remove(const uint64_t cards) {
    m_deck ^= cards;
}

void Deck::reset() {
    m_deck = 0x7FFC7FFC7FFC7FFCULL;
}

int Deck::size() const {
    return std::popcount(m_deck);
}
