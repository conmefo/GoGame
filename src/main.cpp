#include "../../../backend/Board.h"
#include <iostream>

int main() {
    Board board(9);  // start with 9x9
    board.printBoard();

    std::cout << "Place a few stones...\n";
    board.placeStone(4, 4, Color::BLACK);
    board.placeStone(2, 3, Color::WHITE);
    board.placeStone(4, 5, Color::BLACK);

    board.printBoard();

    std::cout << "Change board to 13x13\n";
    board.reset(13);
    board.printBoard();
}
