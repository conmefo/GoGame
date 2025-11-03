#include "../include/Board.h"
#include "../include/Game.h"
#include <iostream>

int main() {
    Board board(9);  // start with 9x9
    Game game(9, GameMode::PLAYER_VS_PLAYER);
    game.makeMove(4, 4);
    game.passTurn();
    game.makeMove(4, 5);
    game.saveGame("save.txt");
    game.loadGame("save.txt");
    return 0;
}
