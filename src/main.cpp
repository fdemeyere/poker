#include "MonteCarlo.h"
#include "InputParser.h"
#include "HandEvaluation.h"
#include <iostream>
#include <optional>
#include <sstream>



int main() {
    std::cout << R"(Input format: ['-' or 0-5 community cards] [2 hole cards] [number of players] [optional: opponent range])" << "\n"
              << R"(Example: - AdAc 3 66+,A8-AK,Q8-QJ,J8-JT,J)" << "\n";
    // Example inputs
    // As3dKc2c8h AdKd 4
    // - AdAc 3

    // As3dKc2c8h AdKd 4 XX-YY X+X+
    // As3dKc2c8h AdKd 4 55-AA 7+J+o
    // As3dKc2c8h AdKd 4 55-AA 7+J+s

    std::optional<MCInput> input {};
    while (!input.has_value()) {
        input = parse_input();
    }

    const uint64_t mc_res {monte_carlo(input.value())};
    const int wins {static_cast<int>((mc_res >> 42) & 0x1FFFFF)};
    const int ties {static_cast<int>((mc_res >> 21) & 0x1FFFFF)};
    const int losses {static_cast<int>(mc_res & 0x1FFFFF)};

    const double win_rate {static_cast<double>(wins) / input.value().num_simulations};
    const double tie_rate {static_cast<double>(ties) / input.value().num_simulations};
    const double loss_rate {static_cast<double>(losses) / input.value().num_simulations};
    std::cout << "Simulations: " << std::to_string(input.value().num_simulations) << "\n"
        << "Win/tie/loss: " << win_rate << "/" << tie_rate << "/" << loss_rate << "\n";
    return 0;
}