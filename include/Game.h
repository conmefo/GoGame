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
};

class Game {
public:
    Game(int boardSize = 9, GameMode mode = GameMode::PLAYER_VS_PLAYER);

    bool makeMove(int x, int y);

    void passTurn();

    void reset();

    void undo();

    void redo();

    bool saveGame(const std::string& filename) const;

    bool loadGame(const std::string& filename);

    const Board& getBoard() const;

    Color getCurrentPlayer() const;

    GameState getGameState() const;

    int getBlackCaptures() const;

    int getWhiteCaptures() const;

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
