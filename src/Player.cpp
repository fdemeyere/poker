//
// Created by legen on 2026-10-17.
//

#include "Player.h"

Player::Player(const Card& c1, const Card& c2) {
    m_cards[0] = c1;
    m_cards[1] = c2;
}


void Player::give_cards(const Card& c1, const Card& c2) {
    m_cards[0] = c1;
    m_cards[1] = c2;
}

std::array<Card, 2> Player::get_cards() const {
    return m_cards;
}
