# Texas Hold'em Monte Carlo Simulator

Utilizing bitwise operations to evaluate hand strength and Monte Carlo simulations to calculate win/tie/loss probabilities.

## Features

* **Bitboard Evaluation:** Uses 64-bit integers to represent cards and decks.
* **0-5 Community Cards:** Simulates odds from any stage of the game.
* **2-23 Players:** Calculates odds against up to 22 opponents. 

## Benchmarks
### Monte Carlo 
**Simulations:** 100 000

**Player count:** 4

**Community cards:** 0 (Pre-flop)

| Benchmark | Time | CPU | Iterations |
| :--- | :--- | :--- | :--- |
| BM_monte_carlo | 17,350,498 ns | 17,013,889 ns | 45 |

0.017s per 100 000 simulations => 170ns/simulation

### Hand evaluation 
**Hand:** High-card (slowest to evaluate)
| Benchmark | Time | CPU | Iterations |
| :--- | :--- | :--- | :--- |
| BM_get_hand_sh | 22.1 ns | 22.0 ns | 32000000 |

22.1ns / hand evaluation => 45 million hand evaluations / second
