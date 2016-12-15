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
#include <cstring>

using namespace std;

const int MAXN = (int)(1e5) + 100;

const int INF = 1e9;

const int fictive_node = 2;
const int root = 1;

struct Node
{
    int Next[26];
    int term;
    int term_achievable;
    int parent, pch;
    int suf;
    Node(int parent_=-1, int parent_char=-1)
            : term(INF),
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
    trie[root].term_achievable = INF;
    trie[fictive_node].term_achievable = INF;
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

char s[MAXN];
int n;

void add_string(int t)
{
    int current = root;
    for (int i = 0; i < n; ++i) {
        int c = s[i] - 'a';
        if (!trie[current].Next[c]) {
            trie[current].Next[c] = (int)trie.size();
            trie.emplace_back(current, c);
        }
        current = trie[current].Next[c];
    }
    trie[current].term = t;
}

int is_term_achievable(int v)
{
    if (trie[v].term_achievable != -1)
        return trie[v].term_achievable;
    return trie[v].term_achievable = min(trie[v].term, is_term_achievable(suf_link(v)));
}

vector<char> color;
string word;
char buffer[110];

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    srand(666);
    pre_build_trie();
    int m = 0;
    vector<pair<string, int>> data;
    for (int t = 1; fgets(buffer, MAXN, stdin); ++t, ++m) {
        n = strlen(buffer) - 3;
        for (int i = 0; i < n; ++i) {
            s[i] =  tolower(buffer[i + 2]);
        }
        s[n] = 0;
        cout << s << endl;
        if (buffer[0] == '?') {
            add_string(t);
        } else {
            data.emplace_back(string(s), t);
        }
    }
    vector<bool> ans(m);
    int current = root;
    for (auto p : data) {
        cout << p.first << " " << p.second << endl;
        for (char c : p.first) {
            current = go(current, c - 'a');
            if (is_term_achievable(c) !=
        }
    }
    return 0;
}
