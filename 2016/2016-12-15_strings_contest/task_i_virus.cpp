//
// Created by ulyanin on 15.12.16.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <stdlib.h>

using namespace std;

const int MAXN = (int)(1e5);

const int fictive_node = 2;
const int root = 1;

struct Node
{
    int Next[2];
    bool term;
    int term_achievable;
    int parent, pch;
    int suf;
    Node(int parent_=-1, int parent_char=-1)
            : term(0),
              term_achievable(-1),
              parent(parent_),
              pch(parent_char),
              suf(0)
    {}
};
vector<Node> trie(3);

void pre_build_trie()
{
    trie[root].suf = fictive_node;
    trie[fictive_node].Next[0] = root;
    trie[fictive_node].Next[1] = root;
    trie[root].term_achievable = 0;
    trie[fictive_node].term_achievable = 0;
}

int go(int v, int c);
int suf_link(int v);

int go(int v, int c)
{
//    cout << "go " << v << endl;
    if (trie[v].Next[c])
        return trie[v].Next[c];
    return trie[v].Next[c] = go(suf_link(v), c);
}

int suf_link(int v)
{
    if (trie[v].suf) {
        return trie[v].suf;
    }
    return trie[v].suf = go(suf_link(trie[v].parent), trie[v].pch);
}


void add_string(const string &s)
{
    int current = root;
    for (char ch : s) {
        int c = ch - '0';
        if (!trie[current].Next[c]) {
            trie[current].Next[c] = (int)trie.size();
            trie.emplace_back(current, c);
        }
        current = trie[current].Next[c];
    }
    trie[current].term = 1;
}

bool is_term_achievable(int v)
{
    if (trie[v].term_achievable != -1)
        return trie[v].term_achievable;
    if (trie[v].term)
        return trie[v].term_achievable = 1;
    return trie[v].term_achievable = is_term_achievable(suf_link(v));
}

void gen(int rest, int v = root)
{
    if (is_term_achievable(v)) {
        return;
    }
    if (rest == 0) {
        cout << "TAK" << endl;
        exit(0);
    }
    int t = rand() & 1;
    gen(rest - 1, go(v, t));
    gen(rest - 1, go(v, 1 ^ t));
}

vector<char> color;
string word;

void dfs(int v)
{
    if (is_term_achievable(v))
        return;
    color[v] = 1;
    for (int c = 0; c <= 1; ++c) {
        int to = go(v, c);
        if (color[to] == 1) {
            cout << "TAK" << endl;
//            cout << word << endl;
            exit(0);
        }
        if (color[to] == -1) {
//            word.push_back('0' + c);
            dfs(to);
//            word.pop_back();
        }
    }
    color[v] = 2;
}

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    srand(666);
    int n;
    scanf("%d\n", &n);
    pre_build_trie();
    int m = 0;
    for (int i = 0; i < n; ++i) {
        string current;
        getline(cin, current);
//        cout << current << endl;
        m += current.size();
        add_string(current);
    }
//    cout << m << endl;
    color.assign(trie.size(), -1);
    for (int i = 1; i < trie.size(); ++i) {
        if (color[i] == -1) {
            dfs(i);
        }
    }
    cout << "NIE" << endl;
    return 0;
}
