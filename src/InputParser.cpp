
#include "HandEvaluation.h"
#include "MonteCarlo.h"
#include "InputParser.h"
#include "Constants.h"
#include <iostream>
#include <optional>
#include <sstream>

std::optional<uint64_t> parse_community_cards(std::string token) {
    uint64_t community_cards {};
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

            const std::optional<uint64_t> new_card {get_card_ext(token.substr(i, 2))};

            if (!new_card.has_value()) return std::nullopt;

            if ((community_cards & new_card.value()) != 0) {
                std::cout << "Invalid input: duplicate card\n\n";
                return std::nullopt;
            }
            community_cards |= new_card.value();
        }
    }
    return community_cards;
}

std::optional<uint64_t> parse_user_cards(std::string token) {
    uint64_t user_cards {};
    if (token.size() != 4) {
        std::cout << "Invalid input: couldn't parse user card\n\n";
        return std::nullopt;
    }
    if (token.size() > 4) {
        std::cout << "Invalid input: too many user cards";
        return std::nullopt;
    }
    for (int i {}; i < 4; i += 2) {
        const std::optional<uint64_t> new_card {get_card_ext(token.substr(i, 2))};

        if (!new_card.has_value()) return std::nullopt;

        if ((user_cards & new_card.value()) != 0) {
            std::cout << "Invalid input: duplicate card\n\n";
            return std::nullopt;
        }
        user_cards |= new_card.value();
    }
    return user_cards;
}

int char_to_rank(char c) {
    if (c >= '2' && c <= '9') return c - '0';
    switch (c) {
        case 'T': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        case 'A': return 14;
        default: return 0;
    }
}

std::optional<std::vector<uint64_t>> parse_pair_specifier(std::string token) {
    std::vector<uint64_t> opp_pocket_range {};

    if (token.size() == 3 && token[2] == '+') {
        const int start_rank {char_to_rank(token[0])};

        for (int r = start_rank; r <= 14; ++r) {
            for (int s1 {}; s1 < 3; ++s1) {
                for (int s2 {s1 + 1}; s2 < 4; ++s2) {
                    const uint64_t card1 {1ULL << (s1 * 16 + r)};
                    const uint64_t card2 {1ULL << (s2 * 16 + r)};
                    opp_pocket_range.push_back(card1 | card2);
                }
            }
        }
    }
    else if (token.size() == 5 && token[2] == '-') {
        const int r1 {char_to_rank(token[0])};
        const int r2 {char_to_rank(token[3])};

        const int start_rank {std::min(r1, r2)};
        const int end_rank {std::max(r1, r2)};


        for (int r = start_rank; r <= end_rank; ++r) {
            for (int s1 {}; s1 < 3; ++s1) {
                for (int s2 {s1 + 1}; s2 < 4; ++s2) {
                    const uint64_t card1 {1ULL << (s1 * 16 + r)};
                    const uint64_t card2 {1ULL << (s2 * 16 + r)};
                    opp_pocket_range.push_back(card1 | card2);
                }
            }
        }
    }
    else {
        return std::nullopt;
    }

    return opp_pocket_range;
}

std::optional<MCInput> parse_input() {
    std::string line;
    std::getline(std::cin, line);
    if (line.find_first_not_of(" \f\n\r\t\v") == std::string::npos) exit(0);

    std::istringstream iss(line);
    uint64_t community_cards {};
    uint64_t user_cards {};
    int num_players {};
    std::vector<uint64_t> opp_pocket_range {};

    std::string token;

    iss >> token;
    std::optional<uint64_t> parsed_community_cards_res = parse_community_cards(token);
    if (!parsed_community_cards_res.has_value()) return std::nullopt;
    community_cards = parsed_community_cards_res.value();


    iss >> token;
    std::optional<uint64_t> parsed_user_cards_res = parse_user_cards(token);
    if (!parsed_user_cards_res.has_value()) return std::nullopt;
    user_cards = parsed_user_cards_res.value();


    if ((community_cards & user_cards) != 0) {
        std::cout << "Invalid input: duplicate card\n\n";
        return std::nullopt;
    }

    // parse player count
    if (!(iss >> num_players)) {
        std::cout << "Invalid input: expected integer player count\n\n";
        return std::nullopt;
    }
    if (num_players < Config::min_player_count || num_players > Config::max_player_count) {
        std::cout << "Invalid input: out of bounds player count\n\n";
        return std::nullopt;
    }

    // parse optional range
    if (!(iss >> token)) return MCInput {community_cards, user_cards, num_players};

    // pair specifiers, range XX-YY or XX+
    if (token[0] == token[1]) {
        const std::optional<std::vector<uint64_t>> parsed_pair_specifier_res = parse_pair_specifier(token);
        if (!parsed_pair_specifier_res.has_value()) return std::nullopt;
        opp_pocket_range = parsed_pair_specifier_res.value();
    }

    return MCInput {community_cards, user_cards, num_players, opp_pocket_range};
}