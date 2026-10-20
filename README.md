# Texas Hold'em Monte Carlo Simulator

Utilizing bitwise operations to evaluate hand strength and Monte Carlo simulations to calculate win/tie/loss probabilities.

## Features

* **Bitboard Evaluation:** Uses 64-bit integers to represent cards and decks.
* **0-5 Community Cards:** Simulates odds from any stage of the game.
* **2-23 Players:** Calculates odds against up to 22 opponents. 

## Benchmark

| Benchmark | Time | CPU | Iterations |
| :--- | :--- | :--- | :--- |
| BM_monte_carlo | 76,670,978 ns | 76,388,889 ns | 9 |

0.077s per 100 000 simulations = 770ns/simulation
