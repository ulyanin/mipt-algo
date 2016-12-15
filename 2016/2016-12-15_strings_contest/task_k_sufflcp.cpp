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

using namespace std;

const int MAXN = (int)(1e5) + 100;

typedef long long ll;
int n;

vector<int> build_lcp(const string &str, const vector<int> &suf)
{
    vector<int> lcp(str.size());
    vector<int> pos(str.size());
    for (int i = 0; i < n; ++i) {
        pos[suf[i]] = i;
    }
    for (int i = 0; i < n; ++i) {
        int p = pos[i];
        if (i != 0) {
            lcp[p] = max(0, lcp[pos[i - 1]] - 1);
        }
        if (p != n - 1) {
            while (str[suf[p] + lcp[p]] == str[suf[p + 1] + lcp[p]])
                ++lcp[p];
        }
    }
    return lcp;
}

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    scanf("%d\n", &n);
    char * buffer = new char[n + 1];
    scanf("%s", buffer);
    std::string str(buffer);
    str.push_back('$');
    vector<int> suf(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &suf[i]);
        --suf[i];
    }
    vector<int> lcp(build_lcp(str, suf));
    for (int i = 0; i < n - 1; ++i) {
        cout << lcp[i] << " ";
    }
    cout << endl;
    return 0;
}
