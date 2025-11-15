#include "../include/Game.h"

#include <fstream>
#include <vector>
#include <queue>

Game::Game(int boardSize, GameMode mode) {
    board = Board(boardSize);
    this->mode = mode;
    reset();
}

void Game::reset() {
    board.reset(board.BOARD_SIZE);
    currentPlayer = StoneColor::BLACK;
    state = GameState::ONGOING;
    blackCaptures = 0;
    whiteCaptures = 0;
    consecutivePasses = 0;
    clearRedoStack();

    while (!undoStack.empty()) {
        undoStack.pop();
    }
}

// tra ve true neu buoc di hop le
bool Game::makeMove(int x, int y) {
    if (state == GameState::FINISHED) {
        return false;
    }

    const std::vector<StoneColor>& boardBeforeMove = board.getGrid();

    bool success = board.placeStone(x, y, currentPlayer);

    if (success) {
        Move move = Move(x, y, currentPlayer);

        const std::vector<StoneColor>& boardAfterMove = board.getGrid();

        for (int i = 0; i < boardAfterMove.size(); ++i) {
            if (boardBeforeMove[i] != boardAfterMove[i] && i != board.idx(x, y)) {
                move.capturedStonesIndices.push_back(i);
            }
        }

        if (currentPlayer == StoneColor::BLACK) {
            blackCaptures += move.capturedStonesIndices.size();
        } else {
            whiteCaptures += move.capturedStonesIndices.size();
        }

        undoStack.push(move);
        clearRedoStack();
        consecutivePasses = 0;
        switchPlayer();
    }

    board.printBoard();
    return success;
}

// skip luot
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

// hoan tac nuoc di
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
        board.forceSetStone(lastMove.x, lastMove.y, StoneColor::EMPTY);

        StoneColor opponentColor =
            (lastMove.playerColor == StoneColor::BLACK) ? StoneColor::WHITE : StoneColor::BLACK;
        for (int index : lastMove.capturedStonesIndices) {
            int y = index / board.BOARD_SIZE;
            int x = index % board.BOARD_SIZE;
            board.forceSetStone(x, y, opponentColor);
        }

        if (lastMove.playerColor == StoneColor::BLACK) {
            blackCaptures -= lastMove.capturedStonesIndices.size();
        } else {
            whiteCaptures -= lastMove.capturedStonesIndices.size();
        }
    }

    currentPlayer = lastMove.playerColor;
    state = GameState::ONGOING;
}

// hoac tac lai nuoc hoan tac
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
            board.forceSetStone(x, y, StoneColor::EMPTY);
        }

        if (moveToRedo.playerColor == StoneColor::BLACK) {
            blackCaptures += moveToRedo.capturedStonesIndices.size();
        } else {
            whiteCaptures += moveToRedo.capturedStonesIndices.size();
        }

        switchPlayer();
    }

    undoStack.push(moveToRedo);
}

std::pair<StoneColor, int> Game::checkTerritory(int position, std::vector<int>& visited) {
    int size = board.BOARD_SIZE;
    std::vector<StoneColor>& grid = board.getGrid();

    int startX = position % size;
    int startY = position / size;

    std::queue<std::pair<int,int>> q;
    q.push({startX, startY});
    visited[position] = 1;

    int territorySize = 0;
    bool touchesBlack = false;
    bool touchesWhite = false;

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        territorySize++;

        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx < 0 || nx >= size || ny < 0 || ny >= size) continue;

            int idx = ny * size + nx;

            if (grid[idx] == StoneColor::EMPTY && !visited[idx]) {
                visited[idx] = 1;
                q.push({nx, ny});
            } else if (grid[idx] == StoneColor::BLACK) {
                touchesBlack = true;
            } else if (grid[idx] == StoneColor::WHITE) {
                touchesWhite = true;
            }
        }
    }

    if (touchesBlack && !touchesWhite) return {StoneColor::BLACK, territorySize};
    if (!touchesBlack && touchesWhite) return {StoneColor::WHITE, territorySize};
    return {StoneColor::EMPTY, 0}; 
}


void Game::calculatePoint() {
    int size = board.BOARD_SIZE;
    std::vector<int> visited(size * size, 0);
    std::vector<StoneColor>& grid = board.getGrid();

    int blackScore = 0;
    int whiteScore = 0;

    for (int i = 0; i < grid.size(); ++i) {
        if (grid[i] == StoneColor::BLACK) blackScore++;
        else if (grid[i] == StoneColor::WHITE) whiteScore++;
    }

    for (int i = 0; i < grid.size(); ++i) {
        if (!visited[i] && grid[i] == StoneColor::EMPTY) {
            auto [owner, territorySize] = checkTerritory(i, visited);
            if (owner == StoneColor::BLACK) blackScore += territorySize;
            else if (owner == StoneColor::WHITE) whiteScore += territorySize;
        }
    }

    blackCaptures += blackScore;
    whiteCaptures += whiteScore;
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

    std::vector<StoneColor>& grid = board.getGrid();
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
        for (int idx : it->capturedStonesIndices) outFile << idx << " ";
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
    currentPlayer = static_cast<StoneColor>(playerInt);

    inFile >> blackCaptures >> whiteCaptures >> consecutivePasses;

    const std::vector<StoneColor>& grid = board.getGrid();
    for (int i = 0; i < grid.size(); ++i) {
        int colorInt;
        inFile >> colorInt;
        int y = i / boardSize;
        int x = i % boardSize;
        board.forceSetStone(x, y, static_cast<StoneColor>(colorInt));
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
        move.playerColor = static_cast<StoneColor>(playerInt);
        inFile >> capturedCount;
        move.capturedStonesIndices.resize(capturedCount);
        for (int j = 0; j < capturedCount; ++j) inFile >> move.capturedStonesIndices[j];
        moveHistory.push_back(move);
    }

    for (auto it = moveHistory.rbegin(); it != moveHistory.rend(); ++it) undoStack.push(*it);

    state = GameState::ONGOING;
    inFile.close();
    return true;
}

Board& Game::getBoard() { return board; }

StoneColor Game::getCurrentPlayer() { return currentPlayer; }

GameState Game::getGameState() { return state; }

int Game::getBlackCaptures() { return blackCaptures; }

int Game::getWhiteCaptures() { return whiteCaptures; }

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == StoneColor::BLACK) ? StoneColor::WHITE : StoneColor::BLACK;
}

void Game::clearRedoStack() {
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}