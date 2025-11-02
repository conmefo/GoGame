#pragma once
#include <vector>
#include <string>


enum class Color : int { EMPTY = 0, BLACK = 1, WHITE = 2 };

inline Color opponent(Color c) {
    if (c == Color::BLACK) return Color::WHITE;
    if (c == Color::WHITE) return Color::BLACK;
    return Color::EMPTY;
}


extern int BOARD_SIZE; 


inline int idx(int x, int y) {
    return y * BOARD_SIZE + x;
}
