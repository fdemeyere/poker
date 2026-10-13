#include <iostream>
#include "Deck.h"
#include <optional>

int main() {
    Deck deck {};
    deck.shuffle();
    std::cout << deck.pop_card().value();
    std::cout << deck.size();

    return 0;
}