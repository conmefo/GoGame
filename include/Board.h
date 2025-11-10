#pragma once
#include "Common.h"
#include <vector>

class Board {
private:
    std::vector<StoneColor> grid;

public:
    int BOARD_SIZE = 9; 
    int idx(int x, int y) const;
    void printBoard() const;
    Board(int size);    
    Board();          
    void reset(int size);           
    void forceSetStone(int x, int y, StoneColor c);
    bool placeStone(int x, int y, StoneColor c);
    int countLiberties(int x, int y, StoneColor c, std::vector<int>& visited, const Board& board, int flag);
    std::vector<int> checkCaptureStones(int x, int y, StoneColor c);
    StoneColor get(int x, int y) const;
    std::vector<StoneColor>& getGrid();
};
