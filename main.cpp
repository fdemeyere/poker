#include "Player.h"
#include "Deck.h"
#include "HandEvaluation.h"
#include <iostream>
#include <optional>
#include <bit>
#include <sstream>

struct MCInput {
    uint64_t community_cards;
    uint64_t user_cards;
    int num_players;
    int num_simulations {100000};
};

std::optional<MCInput> parse_input() {
    std::string line;
    std::getline(std::cin, line);
    if (line.find_first_not_of(" \f\n\r\t\v") == std::string::npos) exit(0);

    std::istringstream iss(line);
    uint64_t community_cards {};
    uint64_t user_cards {};
    int num_players {};

    std::string token;
    bool reading_community_cards {true};
    for (int i {}; i < 5; ++i) {
        if (!(iss >> token)) {
            std::cout << "Invalid input: expected dot\n\n";
            return std::nullopt;
        }
        if (token == ".") {
            reading_community_cards = false;
            break;
        }
        try {
            const uint64_t new_card {get_card_ext(token)};
            if ((community_cards & new_card) != 0) {
                std::cout << "Invalid input: duplicate card\n\n";
                return std::nullopt;
            }
            community_cards |= new_card;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input: " << e.what() << "\n\n";
            return std::nullopt;
        }
    }

    if (reading_community_cards) {
        if (!(iss >> token) || token != ".") {
            std::cout << "Invalid input: expected dot\n\n";
            return std::nullopt;
        }
    }

    for (int i {}; i < 2; ++i) {
        if (!(iss >> token)) {
            std::cout << "Invalid input: expected hole card\n\n";
            return std::nullopt;
        }
        try {
            const uint64_t new_card {get_card_ext(token)};
            if ((user_cards & new_card) != 0) {
                std::cout << "Invalid input: duplicate card\n\n";
                return std::nullopt;
            }
            user_cards |= new_card;
        }
        catch (std::exception& e) {
            std::cout <<  "Invalid input: " << e.what() << "\n\n";
            return std::nullopt;
        }
    }

    if ((community_cards & user_cards) != 0) {
        std::cout << "Invalid input: duplicate card\n\n";
        return std::nullopt;
    }

    if (!(iss >> token) || token != ".") {
        std::cout << "Invalid input: expected dot\n\n";
        return std::nullopt;
    }

    if (!(iss >> num_players)) {
        std::cout << "Invalid input: expected integer player count\n\n";
        return std::nullopt;
    }

    if (num_players < 2 || num_players > 23) {
        std::cout << "Invalid input: out of bounds player count\n\n";
        return std::nullopt;
    }
    return MCInput {community_cards, user_cards, num_players};
}

int monte_carlo(const MCInput& input) {
    int wins {};

    Deck deck {};
    std::array<uint64_t, 23> pockets {input.user_cards};
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

        wins += user_is_winner(pockets, community_cards, input.num_players);
    }
    return wins;
}

int main() {
    std::cout << "Input format: \n0-5 community cards . 2 hole cards . number of players\n\n";
    // Example inputs
    // 5d 10c 14h . 2d 7h . 4
    // 5d 10c 14h 11d 4c . 14c 3h . 4

    std::optional<MCInput> input {};
    while (!input.has_value()) {
        input = parse_input();
    }
    const int wins {monte_carlo(input.value())};
    const double win_rate {static_cast<double>(wins) / input.value().num_simulations};
    std::cout << "Simulations: " << std::to_string(input.value().num_simulations) << "\n" << "Win rate: " << win_rate;
    return 0;
}