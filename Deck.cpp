//
// Created by legen on 2026-10-12.
//

#include "Deck.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <optional>
#include <array>
#include <ranges>
#include <cstddef> // for std::size_t

std::string_view get_rank_name(const Rank rank) {
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

std::string_view get_suit_name(const Suit suit) {
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

Deck::Deck() : m_gen {std::random_device{}()} {
    m_deck.reserve(ranks.size() * suits.size());

    for (const Rank rank : ranks) {
        for (const Suit suit : suits) {
            m_deck.push_back(Card {suit, rank});
        }
    }
}

Card::Card(Suit suit, Rank rank) : rank {rank}, suit {suit} {}

void Deck::shuffle() {
    std::ranges::shuffle(m_deck, m_gen);
}

std::optional<Card> Deck::pop_card() {
    if (m_deck.empty()) {
        return std::nullopt;
    }
    const Card last_card {m_deck.back()};
    m_deck.pop_back();
    return last_card;
}

bool Deck::is_empty() const {
    return m_deck.empty();
}

std::size_t Deck::size() const {
    return m_deck.size();
}
