#include "MonteCarlo.h"
#include "InputParser.h"
#include "HandEvaluation.h"
#include <iostream>
#include <optional>
#include <sstream>



int main() {
    std::cout << R"(Input format: ['-' or 0-5 community cards] [2 hole cards] [number of players] [optional: opponent hole card range])" << "\n";
    // Example inputs
    // As3dKc2c8h AhJd 4 TT+
    // - AdAc 3

    std::optional<MCInput> input {};
    while (!input.has_value()) {
        input = parse_input();
    }

    const MCResult mc_res {monte_carlo(input.value())};

    const double win_rate {static_cast<double>(mc_res.wins) / input.value().num_simulations};
    const double tie_rate {static_cast<double>(mc_res.ties) / input.value().num_simulations};
    const double loss_rate {static_cast<double>(mc_res.losses) / input.value().num_simulations};
    std::cout << "Simulations: " << std::to_string(input.value().num_simulations) << "\n"
        << "Win/tie/loss: " << win_rate << "/" << tie_rate << "/" << loss_rate << "\n";
    return 0;
}