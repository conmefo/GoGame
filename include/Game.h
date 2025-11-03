#include "Board.h"
#include <vector>
#include <stack>
#include <string>

enum class GameMode {
    PLAYER_VS_PLAYER,
    PLAYER_VS_AI
};

enum class GameState {
    ONGOING,
    FINISHED
};
    

struct Move {
    int x;
    int y;
    Color playerColor;
    std::vector<int> capturedStonesIndices; 
    Move (int x= -1, int y= -1, Color c= Color::EMPTY) : x(x), y(y), playerColor(c) {}
};

class Game {
public:
    Game(int boardSize = 9, GameMode mode = GameMode::PLAYER_VS_PLAYER);

    bool makeMove(int x, int y);

    void passTurn();

    void reset();

    void undo();

    void redo();

    bool saveGame(std::string& filename);

    bool loadGame(std::string& filename);

    Board& getBoard();

    Color getCurrentPlayer();

    GameState getGameState();

    int getBlackCaptures();

    int getWhiteCaptures();

private:
    Board board;                      
    Color currentPlayer;              
    GameMode mode;            
    GameState state;             

    int blackCaptures;     
    int whiteCaptures;       
    int consecutivePasses;       

    std::stack<Move> undoStack;   
    std::stack<Move> redoStack;   


    void switchPlayer();
    

    void clearRedoStack();
};
