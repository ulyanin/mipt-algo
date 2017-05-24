#ifndef _N_MOVES_AHEAD_STRATEGY
#define _N_MOVES_AHEAD_STRATEGY

#include "Strategy.h"

#include <map>
#include <unordered_map>
#include <algorithm>

#include <ctime>
#include <climits>

using std::map;
using std::unordered_map;
using std::make_pair;

struct Weight
{
    int weight;
    pair<int, int> step;

    Weight ();

    Weight (int weight, pair<int, int> step);

    bool operator < (const Weight &w) const;

    bool operator > (const Weight &w) const;

    bool operator == (const Weight &w) const;
};

struct State
{
    Color color;
    vector<vector<char> > board;

    bool operator < (const State &s) const;

    bool operator == (const State &s) const;

    State (Color color, const vector<vector<char> > &board);
};

struct StateHash
{
    static const size_t P;

    size_t operator () (const State &s) const;
};


class StrategyNMoves: public IStrategy
{
    unordered_map<State, Weight, StateHash> states;
    clock_t start;

    int weight(int x, int y);

    Weight calcWeight(const State &state);

    State getNewState(int x, int y, const State &state);

    void getEndWeight(const State &state);

    void moveAhead(const State &state, const Weight &parent_weight, int max_depth);

    int depth;

    StrategyNMoves ()
    { }

public:
    StrategyNMoves (int depth)
            : depth(depth)
    { }

    pair<int, int> move(Color color, const vector<vector<char> > &board);
};

bool Weight::operator<(const Weight &w) const
{
    return weight < w.weight;
}

bool Weight::operator>(const Weight &w) const
{
    return operator < (w);
}

bool Weight::operator==(const Weight &w) const
{
    return weight == w.weight && step == w.step;
}

Weight::Weight()
{ }

Weight::Weight(int weight, pair<int, int> step)
        : weight(weight), step(step)
{ }

State::State(Color color, const vector<vector<char> > &board)
        : color(color), board(board)
{ }

bool State::operator<(const State &s) const
{
    return color < s.color || (color == s.color && board < s.board);
}
bool State::operator==(const State &s) const
{
    return color == s.color && board == s.board;
}

const size_t StateHash::P = 59;

size_t StateHash::operator()(const State &s) const
{
    size_t ans = s.color;

    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            ans = ans * P + s.board[x][y];
    return ans;
}

int StrategyNMoves::weight(int x, int y)
{
    if (isAngle(x, y)) {
        return 10;
    }
    return 1;
}

pair<int, int> StrategyNMoves::move(Color color, const vector<vector<char> > &board)
{
    start = clock();

    //~ int inc = (color_ == BLACK);
    int inc = 0;

    int count = countOfEmpty(board);
    if (count <= 20) {
        inc += 1;
    } else if (count <= 15) {
        inc += 8;
    }
    states = unordered_map<State, Weight, StateHash>();
    moveAhead(State(color, board), Weight(INT_MIN, make_pair(-1, -1)), depth + inc);
    return states[State(color, board)].step;
}

void StrategyNMoves::moveAhead(const State &state, const Weight &parent_weight, int max_depth)
{
    if (states.find(state) != states.end()) {
        return;
    }

    if (!max_depth || ((double)(clock() - start) / CLOCKS_PER_SEC > 2.9)) {
        if (!doesMoveExist(state.color, state.board) && !doesMoveExist(anotherColor(state.color), state.board)) {
            getEndWeight(state);
        } else {
            states[state] = calcWeight(state);
        }
        return;
    }

    Weight ans(INT_MIN, make_pair(-1, -1));

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (state.board[x][y] == EMPTY && isMovePossible(x, y, state.color, state.board)) {
                State new_state = getNewState(x, y, state);

                if (new_state.color == state.color) {
                    moveAhead(new_state, parent_weight, max_depth - 1);
                } else {
                    moveAhead(new_state, ans, max_depth - 1);
                }

                int weight = -states[new_state].weight;
                if (new_state.color == state.color) {
                    weight = -weight;
                }

                ans = std::max(ans, Weight(weight, make_pair(x, y)));

                if (new_state.color != state.color && -weight <= parent_weight.weight) {
                    break;
                }
            }
        }
    }

    if (ans.weight == INT_MIN) {
        // the end of game
        getEndWeight(state);
        return;
    }

    states[state] = ans;
}

State StrategyNMoves::getNewState(int x, int y, const State &state)
{
    State new_state(anotherColor(state.color), state.board);
    getMove(x, y, state.color, new_state.board);

    if (!doesMoveExist(new_state.color, new_state.board)) {
        new_state.color = anotherColor(new_state.color);
    }
    return new_state;
}

Weight StrategyNMoves::calcWeight(const State &state)
{
    Weight ans(0, make_pair(-1, -1));
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (state.board[x][y] == EMPTY) {
                ans.weight += weight(x, y) * countAfterMove(x, y, state.color, state.board);
            }
        }
    }

    return ans;
}

void StrategyNMoves::getEndWeight(const State &state)
{
    Weight ans = Weight(0, make_pair(-1, -1));

    ans.weight = getScore(state.color, state.board);
    if (ans.weight > 0) {
        ans.weight += 1000000;
    } else {
        ans.weight -= 1000000;
    }
    states[state] = ans;
}

//IStrategy * getStrategy(int depth = 5)
//{
//    return ;
//}

#endif
