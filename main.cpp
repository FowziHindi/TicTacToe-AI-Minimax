#include <iostream>
#include <vector>
#include <string>
#include <memory>

// Represents a game state
struct TreeNode {
    std::vector<char> board; // Current board state
    int move;                // Move that led to this state
    std::vector<std::unique_ptr<TreeNode>> children; // Possible next states

    TreeNode(const std::vector<char>& b, int m = -1) : board(b), move(m) {}
};

// Adds a new game state to the tree
void insertChild(TreeNode* parent, const std::vector<char>& newBoard, int move) {
    std::unique_ptr<TreeNode> child(new TreeNode(newBoard, move)); // used old unique ptr syntax
    parent->children.push_back(std::move(child));
}

// Formats a board cell for display
std::string formatCell(char cell, int position, bool showNumbers) {
    if (cell == ' ') {
        return showNumbers ? " " + std::to_string(position + 1) + " " : "   ";
    }
    return " " + std::string(1, cell) + " ";
}

// Displays the game board
void printBoard(const std::vector<char>& board) {
    bool showNumbers = true;
    for (char cell : board) {
        if (cell != ' ') {
            showNumbers = false;
            break;
        }
    }
    for (int i = 0; i < 3; i++) {
        int row = i * 3;
        std::cout << formatCell(board[row], row, showNumbers) << "|"
                  << formatCell(board[row + 1], row + 1, showNumbers) << "|"
                  << formatCell(board[row + 2], row + 2, showNumbers) << "\n";
        if (i < 2) {
            std::cout << "---+---+---\n";
        }
    }
}

// Checks if a player has won
bool checkWin(const std::vector<char>& board, char player) {
    const int wins[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };
    for (const auto& win : wins) {
        if (board[win[0]] == player &&
            board[win[1]] == player &&
            board[win[2]] == player) {
            return true;
        }
    }
    return false;
}

// Checks if the board is full
bool isFull(const std::vector<char>& board) {
    for (char cell : board) {
        if (cell == ' ') {
            return false;
        }
    }
    return true;
}

// Checks if the game is over
bool isTerminal(const std::vector<char>& board) {
    return checkWin(board, 'X') || checkWin(board, 'O') || isFull(board);
}

// Scores the board: +10 for AI win, -10 for human win, 0 for draw
int utility(const std::vector<char>& board, char aiSymbol) {
    char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';
    if (checkWin(board, aiSymbol)) {
        return 10;
    }
    if (checkWin(board, humanSymbol)) {
        return -10;
    }
    return 0;
}

// Gets a list of available moves
std::vector<int> getPossibleActions(const std::vector<char>& board) {
    std::vector<int> actions;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            actions.push_back(i + 1);
        }
    }
    return actions;
}

// Makes a move on a new board copy
std::vector<char> applyAction(const std::vector<char>& board, int action, char player) {
    std::vector<char> newBoard = board;
    newBoard[action - 1] = player;
    return newBoard;
}

// AI decision-making with alpha-beta pruning
int alphaBetaMinimax(TreeNode* node, int depth, int alpha, int beta,
                     bool isMaximizing, int& nodeCount,
                     char aiSymbol, char humanSymbol) {
    nodeCount++;
    const std::vector<char>& board = node->board;
    if (isTerminal(board) || depth == 0) {
        return utility(board, aiSymbol);
    }

    char player = isMaximizing ? aiSymbol : humanSymbol;
    std::vector<int> actions = getPossibleActions(board);

    if (isMaximizing) {
        int bestValue = -1000; // A very low number
        for (int action : actions) {
            std::vector<char> newBoard = applyAction(board, action, player);
            insertChild(node, newBoard, action);
            int value = alphaBetaMinimax(node->children.back().get(),
                                         depth - 1, alpha, beta, false,
                                         nodeCount, aiSymbol, humanSymbol);
            bestValue = std::max(bestValue, value);
            alpha = std::max(alpha, value);
            if (beta <= alpha) {
                break;
            }
        }
        return bestValue;
    } else {
        int bestValue = 1000; // A very high number
        for (int action : actions) {
            std::vector<char> newBoard = applyAction(board, action, player);
            insertChild(node, newBoard, action);
            int value = alphaBetaMinimax(node->children.back().get(),
                                         depth - 1, alpha, beta, true,
                                         nodeCount, aiSymbol, humanSymbol);
            bestValue = std::min(bestValue, value);
            beta = std::min(beta, value);
            if (beta <= alpha) {
                break;
            }
        }
        return bestValue;
    }
}

// AI chooses its move
int makeAiMove(const std::vector<char>& board, char aiSymbol, char humanSymbol) {
    std::unique_ptr<TreeNode> root(new TreeNode(board));
    int nodeCount = 1; // Count the root node

    int depth = 0;
    for (char cell : board) {
        if (cell == ' ') {
            depth++;
        }
    }

    int bestValue = -1000;
    int bestMove = -1;
    std::vector<int> actions = getPossibleActions(board);

    for (int action : actions) {
        std::vector<char> newBoard = applyAction(board, action, aiSymbol);
        insertChild(root.get(), newBoard, action);
        int value = alphaBetaMinimax(root->children.back().get(),
                                     depth - 1, -1000, 1000, false,
                                     nodeCount, aiSymbol, humanSymbol);
        if (value > bestValue) {
            bestValue = value;
            bestMove = action;
        }
    }

    std::cout << "AI chose position " << bestMove << ".\n";
    std::cout << "Nodes explored: " << nodeCount << "\n\n";
    return bestMove;
}

// Main game loop
int main() {
    std::cout << "Do you want to play as X or O? (X goes first): ";
    std::string playerChoice;
    std::cin >> playerChoice;
    while (playerChoice != "X" && playerChoice != "O") {
        std::cout << "Invalid choice. Please choose X or O: ";
        std::cin >> playerChoice;
    }
    char playerSymbol = playerChoice[0];
    char aiSymbol = (playerSymbol == 'X') ? 'O' : 'X';

    std::cout << "Welcome to Tic Tac Toe!\n";
    std::cout << "You are playing as " << playerSymbol << ".\n";
    std::cout << "The AI is playing as " << aiSymbol << ".\n";
    std::cout << "Enter a number from 1-9 to make your move:\n\n";

    std::vector<char> board(9, ' ');
    char currentPlayer = 'X';

    while (!isTerminal(board)) {
        printBoard(board);

        if (currentPlayer == playerSymbol) {
            std::cout << "\nYour move (1-9):\n";
            int move;
            bool validMove = false;
            do {
                std::string input;
                std::cin >> input;
                if (input.length() == 1 && input[0] >= '1' && input[0] <= '9') {
                    move = input[0] - '0';
                    if (board[move - 1] == ' ') {
                        board[move - 1] = playerSymbol;
                        validMove = true;
                    } else {
                        std::cout << "That position is taken. Try again.\n";
                    }
                } else {
                    std::cout << "Invalid input. Enter a number 1-9.\n";
                }
                if (!validMove) {
                    std::cout << "\nYour move (1-9):\n";
                }
            } while (!validMove);
        } else {
            std::cout << "\nAI is making a move...\n";
            int move = makeAiMove(board, aiSymbol, playerSymbol);
            board[move - 1] = aiSymbol;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    printBoard(board);
    if (checkWin(board, playerSymbol)) {
        std::cout << "\nCongratulations! You win!\n";
    } else if (checkWin(board, aiSymbol)) {
        std::cout << "\nThe AI wins!\n";
    } else {
        std::cout << "\nIt's a draw!\n";
    }

    return 0;
}