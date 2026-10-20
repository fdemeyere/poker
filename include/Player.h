//
// Created by legen on 2026-10-17.
//

#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include "Deck.h"
#include <array>

class Player {
public:
    Player(const Card& c1, const Card& c2);
    Player() = default;
    void give_cards(const Card& c1, const Card& c2);
    std::array<Card, 2> get_cards() const;
private:
    std::array<Card, 2> m_cards {};
};


#endif //POKER_PLAYER_H