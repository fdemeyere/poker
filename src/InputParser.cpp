//
// Created by legen on 2026-10-20.
//

#include "HandEvaluation.h"
#include "MonteCarlo.h"
#include "InputParser.h"
#include <iostream>
#include <optional>
#include <sstream>

std::optional<MCInput> parse_input() {
    std::string line;
    std::getline(std::cin, line);
    if (line.find_first_not_of(" \f\n\r\t\v") == std::string::npos) exit(0);

    std::istringstream iss(line);
    uint64_t community_cards {};
    uint64_t user_cards {};
    int num_players {};

    std::string token;

    iss >> token;
    if (token != "-") {
        if (token.size() % 2 != 0) {
            std::cout << "Invalid input: couldn't parse community card";
            return std::nullopt;
        }
        if (token.size() > 10) {
            std::cout << "Invalid input: too many community cards";
            return std::nullopt;
        }
        for (int i {}; i < token.size(); i += 2) {
            try {
                const uint64_t new_card {get_card_ext(token.substr(i, 2))};
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
    }

    iss >> token;
    if (token.size() != 4) {
        std::cout << "Invalid input: couldn't parse user card\n\n";
        return std::nullopt;
    }
    if (token.size() > 4) {
        std::cout << "Invalid input: too many user cards";
        return std::nullopt;
    }
    for (int i {}; i < 4; i += 2) {
        try {
            const uint64_t new_card {get_card_ext(token.substr(i, 2))};
            if ((user_cards & new_card) != 0) {
                std::cout << "Invalid input: duplicate card\n\n";
                return std::nullopt;
            }
            user_cards |= new_card;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input: " << e.what() << "\n\n";
            return std::nullopt;
        }
    }

    if ((community_cards & user_cards) != 0) {
        std::cout << "Invalid input: duplicate card\n\n";
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