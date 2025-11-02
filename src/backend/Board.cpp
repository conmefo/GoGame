// src/backend/Board.cpp
#include "include/backend/Common.h"
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <sstream>

using namespace std;

struct Move
{
    int x, y;
    Color player;
    bool pass = false;
};

class Board
{
public:
    Board(int n = 9) : n(n), board(n * n, Color::EMPTY) {}

    // Print board to stdout (simple)
    void print() const
    {
        for (int y = 0; y < n; y++)
        {
            for (int x = 0; x < n; x++)
            {
                Color c = board[idx(x, y, n)];
                char ch = '.';
                if (c == Color::BLACK)
                    ch = 'B';
                if (c == Color::WHITE)
                    ch = 'W';
                cout << ch << ' ';
            }
            cout << '\n';
        }
    }

    bool inBounds(int x, int y) const { return x >= 0 && y >= 0 && x < n && y < n; }

    // Try to place stone. Returns true if move applied; false if illegal.
    // If applied, captures vector filled with captured positions.
    bool placeStone(int x, int y, Color player, vector<int> &captured)
    {
        captured.clear();
        if (!inBounds(x, y))
            return false;
        int id = idx(x, y, n);
        if (board[id] != Color::EMPTY)
            return false;

        // Make a copy of board to test the move
        vector<Color> before = board;
        board[id] = player;

        // Check adjacent opponent groups for captures
        const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
        vector<int> toRemove;
        for (int k = 0; k < 4; k++)
        {
            int nx = x + dx[k], ny = y + dy[k];
            if (!inBounds(nx, ny))
                continue;
            int nid = idx(nx, ny, n);
            if (board[nid] == opponent(player))
            {
                vector<int> group;
                int libs = countLiberties(nx, ny, group);
                if (libs == 0)
                {
                    // capture opponent group
                    toRemove.insert(toRemove.end(), group.begin(), group.end());
                }
            }
        }

        // Remove captured stones
        for (int pos : toRemove)
        {
            board[pos] = Color::EMPTY;
        }

        // Check if our placed stone (its group) has liberties; if none -> illegal move (suicide)
        vector<int> myGroup;
        int myLibs = countLiberties(x, y, myGroup);
        if (myLibs == 0)
        {
            // undo
            board = before;
            return false;
        }

        // fill captured positions output
        captured = toRemove;
        return true;
    }

    // Count liberties of group at x,y and collect positions
    int countLiberties(int sx, int sy, vector<int> &outGroup)
    {
        outGroup.clear();
        if (!inBounds(sx, sy))
            return 0;
        Color col = board[idx(sx, sy, n)];
        if (col == Color::EMPTY)
            return 0;
        vector<char> seen(n * n, 0);
        stack<pair<int, int>> st;
        st.push({sx, sy});
        int liberties = 0;
        const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
        while (!st.empty())
        {
            auto p = st.top();
            st.pop();
            int x = p.first, y = p.second;
            int id = idx(x, y, n);
            if (seen[id])
                continue;
            seen[id] = 1;
            outGroup.push_back(id);
            for (int k = 0; k < 4; k++)
            {
                int nx = x + dx[k], ny = y + dy[k];
                if (!inBounds(nx, ny))
                    continue;
                int nid = idx(nx, ny, n);
                Color c = board[nid];
                if (c == Color::EMPTY)
                    liberties++;
                else if (c == col && !seen[nid])
                    st.push({nx, ny});
            }
        }
        return liberties;
    }

    vector<Color> getBoard() const { return board; }
    int size() const { return n; }
    void setBoard(const vector<Color> &b)
    {
        if ((int)b.size() == n * n)
            board = b;
    }

private:
    int n;
    vector<Color> board;
};
