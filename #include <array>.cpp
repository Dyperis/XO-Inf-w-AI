#include <array>
#include <iostream>

int main() {
    std::array<char, 9> grid{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

    std::cout << "Tic-tac-toe grid (positions 1-9):\n";
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            const int position = row * 3 + col;
            std::cout << ' ' << (grid[position] == ' ' ? char('1' + position) : grid[position]) << ' ';
            if (col < 2) std::cout << '|';
        }
        std::cout << '\n';
        if (row < 2) std::cout << "---+---+---\n";
    }
}