#include "backend/Board.h"
#include <iostream>

Board::Board(int size) {
    BOARD_SIZE = size;
    grid.assign(size * size, Color::EMPTY);
}

void Board::reset(int size) {
    BOARD_SIZE = size;
    grid.assign(size * size, Color::EMPTY);
}

bool Board::placeStone(int x, int y, Color c) {
    if (x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE) return false;
    int index = idx(x, y);
    if (grid[index] != Color::EMPTY) return false;
    grid[index] = c;
    return true;
}

Color Board::get(int x, int y) const {
    return grid[y * BOARD_SIZE + x];
}

void Board::printBoard() const {
    std::cout << "\n   ";
    for (int x = 0; x < BOARD_SIZE; ++x)
        std::cout << x << " ";
    std::cout << "\n";

    for (int y = 0; y < BOARD_SIZE; ++y) {
        std::cout << y << ": ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            Color c = grid[idx(x, y)];
            char symbol = '.';
            if (c == Color::BLACK) symbol = 'B';
            else if (c == Color::WHITE) symbol = 'W';
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}