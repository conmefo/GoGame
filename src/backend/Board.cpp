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
