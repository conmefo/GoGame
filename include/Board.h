#pragma once
#include "Common.h"
#include <vector>

class Board {
private:
    std::vector<Color> grid;

public:
    int BOARD_SIZE = 9; 
    int idx(int x, int y) const;
    void printBoard() const;
    Board(int size);    
    Board();          
    void reset(int size);           
    void forceSetStone(int x, int y, Color c);
    bool placeStone(int x, int y, Color c);
    int countLiberties(int x, int y, Color c, std::vector<int>& visited, const Board& board, int flag);
    std::vector<int> checkCaptureStones(int x, int y, Color c);
    Color get(int x, int y) const;
    std::vector<Color>& getGrid();
};
