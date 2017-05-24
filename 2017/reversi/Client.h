#ifndef _CLIENT
#define _CLIENT

#include "Strategy.h"

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

class Client
{
    IStrategy * strategy_;
    
    Color color_;
    vector<vector<char> > board_;
    
    void badError();
    
    void init();
    
    void move();
    
    void readMove();
    
    Client ();
public:
    
    Client (IStrategy * strategy);
    
    void game();
    
    ~Client ();
};

void Client::badError()
{
    std::cerr << "Something wrong!" << endl;
}

void Client::init()
{
    board_ = std::vector<std::vector<char> >(8, vector<char>(8, EMPTY));

    string s;
    cin >> s;
    if (s == "init")
    {
        string s_color;
        cin >> s_color;

        if (s_color == "black")
            color_ = BLACK;
        else
            color_ = WHITE;
    }
    else
        badError();

    board_[3][3] = board_[4][4] = WHITE;
    board_[3][4] = board_[4][3] = BLACK;
}

void Client::game()
{
    init();
    while (1)
    {
        string s;
        cin >> s;
#ifdef LOCAL
        cerr << s << endl;
#endif
        if (s == "turn")
        {
            move();
        }
        else if (s == "move")
        {
            readMove();
        }
        else if (s == "bad")
        {
            badError();
            break;
        }
        else
            break;
#ifdef LOCAL
        cerr << "----------------------------------------------------------------------------------------" << endl;
#endif
    }
}

Client::~Client()
{
    delete strategy_;
}

Client::Client(IStrategy *strategy)
        : strategy_(strategy)
{
}

Client::Client()
{
}

void Client::move()
{
    pair<int, int> answer = strategy_->move(color_, board_);

    getMove(answer.first, answer.second, color_, board_);
#ifdef LOCAL
    cerr << "move " << answer.first << ' ' << answer.second << endl;
#endif
    cout << "move " << (char)(answer.first + 'a') << ' ' << answer.second + 1 << endl;
}

void Client::readMove()
{
    char c;
    int idx;

    cin >> c >> idx;
#ifdef LOCAL
    cerr << c << ' ' << idx << endl;
#endif
    getMove(c - 'a', idx - 1, anotherColor(color_), board_);
}

#endif
