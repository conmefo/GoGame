#pragma once
#include <vector>
#include <string>


enum class StoneColor : int { EMPTY = 0, BLACK = 1, WHITE = 2 };

inline StoneColor opponent(StoneColor c) {
    if (c == StoneColor::BLACK) return StoneColor::WHITE;
    if (c == StoneColor::WHITE) return StoneColor::BLACK;
    return StoneColor::EMPTY;
}




