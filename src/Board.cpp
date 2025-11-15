#include "../include/Board.h"

#include <iostream>

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int Board::idx(int x, int y) const { return y * BOARD_SIZE + x; }

Board::Board() {
    BOARD_SIZE = 9;
    grid.assign(BOARD_SIZE * BOARD_SIZE, StoneColor::EMPTY);
}

Board::Board(int size) {
    BOARD_SIZE = size;
    grid.assign(size * size, StoneColor::EMPTY);
}

void Board::reset(int size) {
    BOARD_SIZE = size;
    grid.assign(size * size, StoneColor::EMPTY);
}

int Board::countLiberties(int x, int y, StoneColor c, std::vector<int>& visited, const Board& board,
                          int flag) {
    int index = idx(x, y);
    if (visited[index]) return 0;
    visited[index] = flag;

    int liberties = 0;
    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) continue;
        StoneColor neighborStoneColor = get(nx, ny);
        if (neighborStoneColor == StoneColor::EMPTY) {
            liberties += 1;
        } else if (neighborStoneColor == c) {
            liberties += countLiberties(nx, ny, c, visited, board, flag);
        }
    }
    return liberties;
}

void Board::forceSetStone(int x, int y, StoneColor c) {
    if (x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE) return;
    grid[idx(x, y)] = c;
}

std::vector<int> Board::checkCaptureStones(int x, int y, StoneColor c) {
    std::vector<int> captured;
    std::vector<int> visited(BOARD_SIZE * BOARD_SIZE, 0);

    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) continue;

        StoneColor neighborStoneColor = get(nx, ny);
        if (neighborStoneColor != StoneColor::EMPTY && neighborStoneColor != c) {
            int liberties = countLiberties(nx, ny, neighborStoneColor, visited, *this, dir + 1);
            if (liberties == 0) {
                for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
                    if (visited[i] == dir + 1) {
                        captured.push_back(i);
                    }
                }
            }
        }
    }


    return captured;
}

bool Board::placeStone(int x, int y, StoneColor c) {
    if (x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE) return false;
    int index = idx(x, y);

    if (grid[index] != StoneColor::EMPTY) return false;

    grid[index] = c;
    std::vector<int> captured = checkCaptureStones(x, y, c);
    if (captured.empty()) {
        std::vector<int> visited(BOARD_SIZE * BOARD_SIZE, 0);
        if (countLiberties(x, y, c, visited, *this, 1) == 0) {  // suicide check
            std::cout << "suicide move\n";
            grid[index] = StoneColor::EMPTY;
            return false;
        }
        return true;
    } else {

        for (int i : captured) {
            grid[i] = StoneColor::EMPTY;
        }
        return true;
    }

    return true;
}

StoneColor Board::get(int x, int y) const { return grid[y * BOARD_SIZE + x]; }

void Board::printBoard() const {
    std::cout << "\n   ";
    for (int x = 0; x < BOARD_SIZE; ++x) std::cout << x << " ";
    std::cout << "\n";

    for (int y = 0; y < BOARD_SIZE; ++y) {
        std::cout << y << ": ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            StoneColor c = grid[idx(x, y)];
            char symbol = '.';
            if (c == StoneColor::BLACK)
                symbol = 'B';
            else if (c == StoneColor::WHITE)
                symbol = 'W';
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

std::vector<StoneColor>& Board::getGrid() { return grid; }