#include "../include/Game.h"
#include <fstream>
#include <vector>


Game::Game(int boardSize, GameMode mode) {
    board = Board(boardSize);
    this->mode = mode;
    reset(); 
}


void Game::reset() {
    board.reset(board.BOARD_SIZE); 
    currentPlayer = Color::BLACK;
    state = GameState::ONGOING;
    blackCaptures = 0;
    whiteCaptures = 0;
    consecutivePasses = 0;
    clearRedoStack();

    while (!undoStack.empty()) {
        undoStack.pop();
    }
}


bool Game::makeMove(int x, int y) {
    if (state == GameState::FINISHED) {
        return false;
    }

    const std::vector<Color>& boardBeforeMove = board.getGrid();
    
    bool success = board.placeStone(x, y, currentPlayer);

    if (success) {
        Move move = Move(x, y, currentPlayer);

        const std::vector<Color>& boardAfterMove = board.getGrid();

        for (int i = 0; i < boardAfterMove.size(); ++i) {
            if (boardBeforeMove[i] != boardAfterMove[i] && i != board.idx(x, y)) {
                move.capturedStonesIndices.push_back(i);
            }
        }

        if (currentPlayer == Color::BLACK) {
            blackCaptures += move.capturedStonesIndices.size();
        } else {
            whiteCaptures += move.capturedStonesIndices.size();
        }
        
        undoStack.push(move);
        clearRedoStack(); 
        consecutivePasses = 0;
        switchPlayer();
    }

    return success;
}

void Game::passTurn() {
    if (state == GameState::FINISHED) {
        return;
    }

    Move passMove = Move(-1, -1, currentPlayer);
    undoStack.push(passMove);
    clearRedoStack();

    consecutivePasses++;
    if (consecutivePasses >= 2) {
        state = GameState::FINISHED; 
    } else {
        switchPlayer();
    }
}


void Game::undo() {
    if (undoStack.empty()) {
        return; 
    }

    Move lastMove = undoStack.top();
    undoStack.pop();
    redoStack.push(lastMove);

    if (lastMove.x == -1) { 
        consecutivePasses--;
    } else { 
        board.forceSetStone(lastMove.x, lastMove.y, Color::EMPTY);

        Color opponentColor = (lastMove.playerColor == Color::BLACK) ? Color::WHITE : Color::BLACK;
        for (int index : lastMove.capturedStonesIndices) {
            int y = index / board.BOARD_SIZE;
            int x = index % board.BOARD_SIZE;
            board.forceSetStone(x, y, opponentColor);
        }

        if (lastMove.playerColor == Color::BLACK) {
            blackCaptures -= lastMove.capturedStonesIndices.size();
        } else {
            whiteCaptures -= lastMove.capturedStonesIndices.size();
        }
    }
    
    currentPlayer = lastMove.playerColor; 
    state = GameState::ONGOING; 
}

void Game::redo() {
    if (redoStack.empty()) {
        return; 
    }

    Move moveToRedo = redoStack.top();
    redoStack.pop();

    if (moveToRedo.x == -1) { 
        passTurn(); 
        undoStack.pop(); 
    } else { 
        board.forceSetStone(moveToRedo.x, moveToRedo.y, moveToRedo.playerColor);
        
        for (int index : moveToRedo.capturedStonesIndices) {
            int y = index / board.BOARD_SIZE;
            int x = index % board.BOARD_SIZE;
            board.forceSetStone(x, y, Color::EMPTY);
        }

        if (moveToRedo.playerColor == Color::BLACK) {
            blackCaptures += moveToRedo.capturedStonesIndices.size();
        } else {
            whiteCaptures += moveToRedo.capturedStonesIndices.size();
        }
        
        switchPlayer();
    }

    undoStack.push(moveToRedo);
}


bool Game::saveGame(std::string filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;

    outFile << board.BOARD_SIZE << std::endl;
    outFile << static_cast<int>(mode) << std::endl;
    outFile << static_cast<int>(currentPlayer) << std::endl;
    outFile << blackCaptures << std::endl;
    outFile << whiteCaptures << std::endl;
    outFile << consecutivePasses << std::endl;

    const std::vector<Color>& grid = board.getGrid();
    for (int i = 0; i < grid.size(); ++i)
        outFile << static_cast<int>(grid[i]) << (i == grid.size() - 1 ? "" : " ");
    outFile << std::endl;

    std::stack<Move> tempStack = undoStack;
    std::vector<Move> history;
    while (!tempStack.empty()) {
        history.push_back(tempStack.top());
        tempStack.pop();
    }

    outFile << history.size() << std::endl;
    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        outFile << it->x << " " << it->y << " " << static_cast<int>(it->playerColor) << " ";
        outFile << it->capturedStonesIndices.size() << " ";
        for (int idx : it->capturedStonesIndices)
            outFile << idx << " ";
        outFile << std::endl;
    }

    outFile.close();
    return true;
}

bool Game::loadGame(std::string filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;

    int boardSize;
    inFile >> boardSize;
    board.reset(boardSize);

    int modeInt, playerInt;
    inFile >> modeInt >> playerInt;
    mode = static_cast<GameMode>(modeInt);
    currentPlayer = static_cast<Color>(playerInt);

    inFile >> blackCaptures >> whiteCaptures >> consecutivePasses;

    const std::vector<Color>& grid = board.getGrid();
    for (int i = 0; i < grid.size(); ++i) {
        int colorInt;
        inFile >> colorInt;
        int y = i / boardSize;
        int x = i % boardSize;
        board.forceSetStone(x, y, static_cast<Color>(colorInt));
    }

    while (!undoStack.empty()) undoStack.pop();
    clearRedoStack();

    int historySize;
    inFile >> historySize;

    std::vector<Move> moveHistory;
    for (int i = 0; i < historySize; ++i) {
        Move move;
        int playerInt, capturedCount;
        inFile >> move.x >> move.y >> playerInt;
        move.playerColor = static_cast<Color>(playerInt);
        inFile >> capturedCount;
        move.capturedStonesIndices.resize(capturedCount);
        for (int j = 0; j < capturedCount; ++j)
            inFile >> move.capturedStonesIndices[j];
        moveHistory.push_back(move);
    }

    for (auto it = moveHistory.rbegin(); it != moveHistory.rend(); ++it)
        undoStack.push(*it);

    state = GameState::ONGOING;
    inFile.close();
    return true;
}



Board& Game::getBoard() {
    return board;
}

Color Game::getCurrentPlayer() {
    return currentPlayer;
}

GameState Game::getGameState() {
    return state;
}

int Game::getBlackCaptures() {
    return blackCaptures;
}

int Game::getWhiteCaptures() {
    return whiteCaptures;
}


void Game::switchPlayer() {
    currentPlayer = (currentPlayer == Color::BLACK) ? Color::WHITE : Color::BLACK;
}

void Game::clearRedoStack() {
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}