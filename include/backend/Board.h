#pragma once
#include "Common.h"
#include <vector>

class Board {
private:
    std::vector<Color> grid;

public:
    void printBoard() const;
    Board(int size);              
    void reset(int size);           
    bool placeStone(int x, int y, Color c);
    Color get(int x, int y) const;
};
