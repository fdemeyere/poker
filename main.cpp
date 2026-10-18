#include "Player.h"
#include "Deck.h"
#include "HandEvaluation.h"
#include <iostream>
#include <optional>
#include <bit>

struct MCInput {
    uint64_t community_cards;
    uint64_t user_cards;
    int num_players;
    int num_simulations {100000};
};

std::optional<MCInput> parse_input() {
    uint64_t community_cards {};
    uint64_t user_cards {};
    int num_players {};

    bool reading_community_cards {true};
    for (int i {}; i < 5; ++i) {
        std::string opt_community_card {};
        std::cin >> opt_community_card;
        if (opt_community_card == ".") {
            reading_community_cards = false;
            break;
        }
        try {
            community_cards |= get_card_ext(opt_community_card);
        }
        catch (std::exception& e) {
            std::cout << "Invalid input: " << e.what() << '\n';
            return std::nullopt;
        }
    }

    if (reading_community_cards) {
        char ch;
        std::cin >> ch;
        if (ch != '.') {
            std::cout << "Invalid input: Dot not found";
            return std::nullopt;
        }
    }

    for (int i {}; i < 2; ++i) {
        std::string user_card {};
        std::cin >> user_card;
        try {
            user_cards |= get_card_ext(user_card);
        }
        catch (std::exception& e) {
            std::cout << "Invalid input: " << e.what() << '\n';
            return std::nullopt;
        }
    }

    char ch;
    std::cin >> ch;
    if (ch != '.') {
        std::cout << "Invalid input: Dot not found";
        return std::nullopt;
    }

    std::cin >> num_players;
    if (num_players < 2) {
        std::cout << "Invalid input: At least 2 players";
        return std::nullopt;
    }
    return MCInput {community_cards, user_cards, num_players};
}

int monte_carlo(const MCInput& input) {
    int wins {};

    Deck deck {};
    std::array<uint64_t, 4> pockets {input.user_cards};
    uint64_t community_cards {};
    for (int i {}; i < input.num_simulations; ++i) {
        deck.reset();
        deck.remove(input.user_cards);
        community_cards = input.community_cards;
        deck.remove(community_cards);

        for (int j {1}; j < input.num_players; ++j) {
            pockets[j] = deck.pop(2);
        }
        for (int j {}; j < 5 - std::popcount(input.community_cards); ++j) {
            community_cards |= deck.pop(1);
        }

        wins += user_is_winner(pockets, community_cards);
    }
    return wins;
}

int main() {
    // Example inputs
    // 5d 10c 14h . 2d 7h . 4
    // 5d 10c 14h 11d 4c . 14c 3h . 4

    std::optional<MCInput> input {};
    while (!input.has_value()) {
        input = parse_input();
    }
    const int wins {monte_carlo(input.value())};
    const double win_rate {static_cast<double>(wins) / input.value().num_simulations};
    std::cout << "Simulations: " + std::to_string(input.value().num_simulations) << "\n" << "Win rate: " << win_rate;
    return 0;
}