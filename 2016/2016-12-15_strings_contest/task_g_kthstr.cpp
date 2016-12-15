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

using namespace std;

const int MAXN = (int)(1e5) + 10;

struct Node
{
    map<char, Node *> edge;
    int cnt;
    int term;

    Node()
            : cnt(0)
            , term(0)
    {}

    Node * go(char c) {
        return exist(c) ? edge[c] : nullptr;
    }

    void insert(char c) {
        edge[c] = new Node();
    }

    bool exist(char c) {
        return edge.find(c) != edge.end();
    }
};

char buffer[MAXN];

void insert(Node * T, const string &str, int i = 0)
{
    if (i == str.size()) {
        if (!T->term)
            T->cnt++;
        T->term = 1;
        return;
    }
    char c = str[i];
    if (!T->exist(c)) {
        T->insert(c);
    }
    T->cnt -= T->go(c)->cnt;
    insert(T->go(c), str, i + 1);
    T->cnt += T->go(c)->cnt;
}

void get_kth(Node * T, int k, string &ans)
{
    int sum = 0;
    if (T->term) {
        if (k == 0) {
            return;
        }
        k--;
    }
    for (auto p : T->edge) {
        if (k < sum + p.second->cnt) {
            ans.push_back(p.first);
            get_kth(p.second, k - sum, ans);
            return;
        }
        sum += p.second->cnt;
    }
}

void print(Node * T, int depth=0)
{
    if (!T)
        return;
    for (auto p : T->edge) {
        for (int i = 0; i < depth; ++i)
            cout << '\t';
        cout << p.first << " " << p.second->cnt << endl;
        print(p.second, depth + 1);
    }
}

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    int n;
    scanf("%d\n", &n);
    Node * bor = new Node();
    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        string str(buffer);
        if (isdigit(str[0])) {
            int k = stoi(str);
            string ans;
            get_kth(bor, k - 1, ans);
            cout << ans << endl;
        } else {
            insert(bor, str);
        }
//        print(bor);
//        cout << "___" << endl;
    }
    return 0;
}
