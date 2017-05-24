#ifndef _STRATEGY
#define _STRATEGY

#include <vector>
#include <iostream>

#include <cassert>

using std::vector;
using std::pair;

using std::cerr;
using std::endl;

const int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
const int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

enum Color
{
    BLACK,
    WHITE,
    EMPTY
};

class IStrategy
{
public:
    virtual pair<int, int> move(Color color, const vector<vector<char> > &board) = 0;
    virtual ~IStrategy ()
    { }
};

Color anotherColor(Color c)
{
    assert(c != EMPTY);
    return static_cast<Color>(c ^ 1);
}

bool isOnBoard(int i)
{
    return i < 8;
}

bool canMove(int x, int y, int i, Color current_color, const vector<vector<char> > &board)
{
    return isOnBoard(x + dx[i]) && isOnBoard(y + dy[i]) && 
    board[x + dx[i]][y + dy[i]] == anotherColor(current_color);
}

int howManyMove(int x, int y, int i, Color current_color, const vector<vector<char> > &board)
{
    int ans = 0;
    while (canMove(x, y, i, current_color, board)) {
        x += dx[i];
        y += dy[i];
        ans++;
    }
    
    if (isOnBoard(x + dx[i]) && isOnBoard(y + dy[i]) && board[x + dx[i]][y + dy[i]] == current_color) {
        return ans;
    }
    return 0;
}

void getMove(int x, int y, Color current_color, vector<vector<char> > &board)
{
    for (int i = 0; i < 8; ++i) {
        int num = howManyMove(x, y, i, current_color, board);
        
        for (int d = 0; d <= num; ++d) {
            board[x + d * dx[i]][y + d * dy[i]] = current_color;
        }
    }
}

bool isMovePossible(int x, int y, Color color, const vector<vector<char> > &board)
{
    if (board[x][y] != EMPTY) {
        return 0;
    }
    
    for (int i = 0; i < 8; ++i) {
        if (howManyMove(x, y, i, color, board) > 0) {
            return 1;
        }
    }
    return 0;
}

int countAfterMove(int x, int y, Color color, const vector<vector<char> > &board)
{
    int ans = 0;
    
    for (int i = 0; i < 8; ++i) {
        ans += howManyMove(x, y, i, color, board);
    }
    return ans;
}

bool doesMoveExist(Color color, const vector<vector<char> > &board)
{
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (isMovePossible(x, y, color, board)) {
                return 1;
            }
        }
    }
    return 0;
}

bool isAngle(int x, int y)
{
    return (x == 0 && y == 0) || (x == 7 && y == 0) || (x == 0 && y == 7) || (x == 7 && y == 7);
}

bool isSide(int x, int y)
{
    return x == 0 || x == 7 || y == 0 || x == 7; 
}

int getScore(Color color, const vector<vector<char> > &board)
{
    int score = 0;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (board[x][y] == color) {
                score += 1;
            } else if (board[x][y] != EMPTY) {
                score -= 1;
            }
        }
    }
    return score;
}

int countOfEmpty(const vector<vector<char> > &board)
{
    int ans = 0;
    
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            ans += (board[x][y] == EMPTY);
        }
    }
    
    return ans;
}

void printBoard(const vector<vector<char> > &board)
{
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cerr << (int) board[j][i] << ' ';
        }
        cerr << endl;
    }
    cerr << endl;
}

#endif
