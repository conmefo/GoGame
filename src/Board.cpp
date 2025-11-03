#include "../include/Board.h"
#include <iostream>

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int Board::idx(int x, int y) const{
    return y * BOARD_SIZE + x;
}


Board::Board(int size) {
    BOARD_SIZE = size;
    grid.assign(size * size, Color::EMPTY);
}

void Board::reset(int size) {
    BOARD_SIZE = size;
    grid.assign(size * size, Color::EMPTY);
}

int Board::countLiberties(int x, int y, Color c, std::vector<int>& visited, const Board& board, int flag) {
    if (x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE) return 0;
    int index = idx(x, y);
    if (visited[index]) return 0;
    visited[index] = flag;

    Color current = board.get(x, y);
    if (current == Color::EMPTY) return 1;
    if (current != c) return 0;

    int liberties = 0;
    for (int dir = 0; dir < 4; ++dir) {
        liberties += countLiberties(x + dx[dir], y + dy[dir], c, visited, board, flag);
    }
    return liberties;
}

std::vector<int> Board::checkCaptureStones(int x, int y, Color c) {
    std::vector<int> captured;
    std::vector<int> visited(BOARD_SIZE * BOARD_SIZE, 0);


    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        if (nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) continue;

        Color neighborColor = get(nx, ny);
        if (neighborColor != Color::EMPTY && neighborColor != c) {
            int liberties = countLiberties(nx, ny, neighborColor, visited, *this, dir + 1);
            if (liberties == 0) {
                for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
                    if (visited[i] == dir + 1) {
                        captured.push_back(i);
                    }
                }
            }
        }
    }

    std::cout << "Captured stones count: " << captured.size() << "\n";

    return captured;
}

bool Board::placeStone(int x, int y, Color c) {
    if (x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE) return false;
    int index = idx(x, y);
    
    if (grid[index] != Color::EMPTY) return false;
    
    grid[index] = c;
    std::vector<int> captured = checkCaptureStones(x, y, c);
    if (captured.empty()) {
        std::vector<int> visited(BOARD_SIZE * BOARD_SIZE, 0);
        if (countLiberties(x, y, c, visited, *this, 1) == 0) { // suicide check
            std::cout << "suicide move\n";
            grid[index] = Color::EMPTY;
            return false;
        }
        return true;
    } else {
        for (int i : captured) {
            grid[i] = Color::EMPTY;
        }
        return true;
    }

    return true;
}

Color Board::get(int x, int y) const {
    return grid[y * BOARD_SIZE + x];
}

void Board::printBoard() const {
    std::cout << "\n   ";
    for (int x = 0; x < BOARD_SIZE; ++x)
        std::cout << x << " ";
    std::cout << "\n";

    for (int y = 0; y < BOARD_SIZE; ++y) {
        std::cout << y << ": ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            Color c = grid[idx(x, y)];
            char symbol = '.';
            if (c == Color::BLACK) symbol = 'B';
            else if (c == Color::WHITE) symbol = 'W';
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}