#include "../include/Board.h"
#include <iostream>

int main() {
    Board board(9);  // start with 9x9
    board.printBoard();

    std::cout << "Place a few stones...\n";
    board.placeStone(1, 0, Color::BLACK);
    board.placeStone(0, 1, Color::BLACK);
    board.placeStone(0, 0, Color::WHITE);

    board.printBoard();
}
