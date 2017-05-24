#ifndef _FIRSTS_STRATEGY
#define _FIRSTS_STRATEGY

#include "Strategy.h"
#include <utility>

class SimpleStrategy: public IStrategy
{
public:
    pair<int, int> move(Color color, const vector<vector<char> > &board)
    {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == EMPTY) {
                    for (int d = 0; d < 8; ++d) {
                        if (howManyMove(i, j, d, color, board)) {
                            return std::make_pair(i, j);
                        }
                    }
                }
            }
        }
        assert(0);
    }
};

IStrategy * getStrategy()
{
    return new SimpleStrategy();
}

#endif
