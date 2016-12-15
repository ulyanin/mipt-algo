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

const int MAXN = (int)(1e6) + 100;

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
    for (int i = 0; i < 26; ++i) {
        trie[fictive_node].Next[i] = root;
    }
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

void add_string(string s)
{
    int current = root;
    for (char ch : s) {
        int c = ch - 'a';
        if (!trie[current].Next[c]) {
            trie[current].Next[c] = (int)trie.size();
            trie.emplace_back(current, c);
        }
        current = trie[current].Next[c];
    }
    trie[current].term++;
}

int num_term_achievable(int v)
{
    if (trie[v].term_achievable != -1)
        return trie[v].term_achievable;
    return trie[v].term_achievable = trie[v].term + num_term_achievable(suf_link(v));
}

char buffer[MAXN];
vector<vector<pair<int, int>>> graph;
long long ans = 0;

void gen(int trie_v, int v, int pc=-1)
{
    ans += num_term_achievable(trie_v);
    for (auto to : graph[v]) {
        gen(go(trie_v, to.second), to.first, to.second);
    }
}

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    pre_build_trie();
    int n;
    scanf("%d", &n);
    graph.resize(n + 1);
    for (int i = 0; i < n; ++i) {
        int m;
        scanf("%d", &m);
        for (int j = 0; j < m; ++j) {
            int to;
            char c;
            scanf(" %d %c", &to, &c);
            graph[i + 1].emplace_back(to, c - 'a');
        }
    }
    int m;
    scanf("%d\n", &m);
    for (int i = 0; i < m; ++i) {
        scanf("%s\n", buffer);
        add_string(string(buffer));
    }
    gen(1, 1);
    cout << ans << endl;

    return 0;
}
