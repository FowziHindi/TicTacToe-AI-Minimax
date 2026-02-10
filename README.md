# Tic-Tac-Toe AI (Minimax and Alpha-Beta Pruning)

### Project Overview
This project is a C++ implementation of a Tic-Tac-Toe game featuring a strategic AI. It was developed to demonstrate mastery of tree-based data structures and recursive decision-making algorithms.

### Technical Highlights
* **N-ary Tree Architecture**: Implemented a custom tree structure where each node represents a game state. Each node encapsulates a full snapshot of the board and the specific move taken to reach that configuration.
* **Minimax Algorithm**: Employs a recursive strategy to determine the optimal move set by simulating all possible future game outcomes.
* **Alpha-Beta Pruning**: Optimized the search process by pruning branches that do not influence the final decision, significantly reducing the number of nodes explored.
* **Smart Utility Function**: The AI prioritizes winning states (+10) and draws (0) while strictly avoiding loss states (-10).
* **Memory Management**: Utilizes modern C++ memory management practices (std::unique_ptr) for efficient tree construction and destruction.

### Implementation Details
* **Language**: C++
* **Traversal**: Post-order (Depth-First Search)
* **Search Optimization**: Alpha-Beta Pruning
* **Tie-Breaking**: Preference for the move occurring first in the iteration when utility scores are identical.

### How to Run

1. **Compile the source**:

```bash
   g++ main.cpp -o TicTacToe
```
2. **Run the executable**:

    Linux / macOS: ./TicTacToe

    Windows: TicTacToe.exe

3. **Gameplay**:
   
 Follow the on-screen prompts to choose your symbol (X or O) and enter your moves using the 1-9 grid system.
