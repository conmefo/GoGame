#pragma once
#include <vector>
#include <string>


enum class Color : int { EMPTY = 0, BLACK = 1, WHITE = 2 };

inline Color opponent(Color c) {
    if (c == Color::BLACK) return Color::WHITE;
    if (c == Color::WHITE) return Color::BLACK;
    return Color::EMPTY;
}


int BOARD_SIZE = 9; // default board size


inline int idx(int x, int y) {
    return y * BOARD_SIZE + x;
}
