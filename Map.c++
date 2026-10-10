#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator<(const Position& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }
};

struct PositionHash {
    std::size_t operator()(const Position& p) const {
        std::size_t h1 = std::hash<int>{}(p.row);
        std::size_t h2 = std::hash<int>{}(p.col);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

class InfiniteTicTacToe {
public:
    InfiniteTicTacToe() : currentPlayer_('X') {}

    bool makeMove(int row, int col) {
        Position pos{row, col};
        if (board_.find(pos) != board_.end()) {
            std::cout << "That cell is already occupied.\n";
            return false;
        }

        board_[pos] = currentPlayer_;

        if (hasWinningLine(pos, currentPlayer_)) {
            winner_ = currentPlayer_;
            gameOver_ = true;
        } else if (board_.size() == maxCells_) {
            winner_ = ' ';
            gameOver_ = true;
        } else {
            currentPlayer_ = (currentPlayer_ == 'X') ? 'O' : 'X';
        }

        return true;
    }

    bool isGameOver() const {
        return gameOver_;
    }

    char winner() const {
        return winner_;
    }

    void printBoard() const {
        if (board_.empty()) {
            std::cout << "Board is empty. Use coordinates like: 0 0\n";
            return;
        }

        int minRow = std::numeric_limits<int>::max();
        int maxRow = std::numeric_limits<int>::min();
        int minCol = std::numeric_limits<int>::max();
        int maxCol = std::numeric_limits<int>::min();

        for (const std::pair<const Position, char>& entry : board_) {
            const Position& pos = entry.first;
            minRow = std::min(minRow, pos.row);
            maxRow = std::max(maxRow, pos.row);
            minCol = std::min(minCol, pos.col);
            maxCol = std::max(maxCol, pos.col);
        }

        const int padding = 1;
        const int rowStart = minRow - padding;
        const int rowEnd = maxRow + padding;
        const int colStart = minCol - padding;
        const int colEnd = maxCol + padding;

        std::cout << "\nCurrent board:\n";
        std::cout << "    ";
        for (int col = colStart; col <= colEnd; ++col) {
            std::cout << std::setw(3) << col;
        }
        std::cout << '\n';

        for (int row = rowStart; row <= rowEnd; ++row) {
            std::cout << std::setw(3) << row << " ";
            for (int col = colStart; col <= colEnd; ++col) {
                Position pos{row, col};
                auto it = board_.find(pos);
                char cell = it != board_.end() ? it->second : '.';
                std::cout << std::setw(3) << cell;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    void printTurn() const {
        std::cout << "Player " << currentPlayer_ << " move. Enter row and column: ";
    }

private:
    static constexpr std::size_t maxCells_ = 1000000;

    bool hasWinningLine(const Position& pos, char mark) const {
        static const std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 0}, {1, 1}, {1, -1}
        };

        for (std::size_t i = 0; i < directions.size(); ++i) {
            const std::pair<int, int>& direction = directions[i];
            const int dr = direction.first;
            const int dc = direction.second;
            int count = 1;
            count += countInDirection(pos, mark, dr, dc);
            count += countInDirection(pos, mark, -dr, -dc);
            if (count >= 7) {
                return true;
            }
        }
        return false;
    }

    int countInDirection(const Position& start, char mark, int dr, int dc) const {
        int count = 0;
        for (int step = 1; step <= 2; ++step) {
            Position next{start.row + dr * step, start.col + dc * step};
            auto it = board_.find(next);
            if (it != board_.end() && it->second == mark) {
                ++count;
            } else {
                break;
            }
        }
        return count;
    }

    std::unordered_map<Position, char, PositionHash> board_;
    char currentPlayer_;
    char winner_ = ' ';
    bool gameOver_ = false;
};

int main() {
    std::cout << "Infinite Tic-Tac-Toe\n";
    std::cout << "Rules: place three marks in a row in any direction.\n";
    std::cout << "Coordinates are entered as: row col\n";
    std::cout << "Example: 0 0, 1 0, -1 2\n\n";

    InfiniteTicTacToe game;
    while (!game.isGameOver()) {
        game.printBoard();
        game.printTurn();

        int row = 0;
        int col = 0;
        if (!(std::cin >> row >> col)) {
            std::cout << "Invalid input. Please enter integer coordinates.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (!game.makeMove(row, col)) {
            continue;
        }
    }

    game.printBoard();
    char winner = game.winner();
    if (winner == ' ') {
        std::cout << "The game ended in a draw.\n";
    } else {
        std::cout << "Player " << winner << " wins!\n";
    }

    return 0;
}