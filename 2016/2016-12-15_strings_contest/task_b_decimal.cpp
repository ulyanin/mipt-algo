//
// Created by ulyanin on 15.12.16.

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>

typedef long long ll;

using namespace std;

vector<int> z_function(const string &str)
{
    std::vector<int> z(str.size());
    int n = str.size();
    for (int i = 1, l = 0, r = 1; i < n; ++i) {
        if (i <= r) {
            z[i] = min(r - i, z[i - l]);
        }
        while (i + z[i] < n && str[z[i]] == str[z[i] + i])
            ++z[i];
        if (i + z[i] > r) {
            r = i + z[i];
            l = i;
        }
    }
    return z;
}

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    char buffer[1000000 + 10];
    char d0;
    scanf("%c.%s", &d0, buffer);
    string str(buffer);
    reverse(str.begin(), str.end());
    vector<int> z(z_function(str));
#ifdef LOCAL
    for (char val : str) {
        cout << val << " ";
    }
    cout << endl;
    for (int val : z) {
        cout << val << " ";
    }
    cout << endl;
#endif
    string per(""), rest(str);
    int len = str.size();
    int n = str.length();
    for (size_t i = 0; i < str.size(); ++i) {
        if (z[i] != 0 && i + (n - i - z[i]) < len) {
            per = str.substr(z[i], i);
            reverse(per.begin(), per.end());
            rest = str.substr(i + z[i]);
            len = n - z[i];
        }
    }
    reverse(rest.begin(), rest.end());
    cout << d0 << '.';
    if (per.empty()) {
        cout << '(' << rest << ')' << endl;
    } else {
        cout << rest << '(' << per << ')' << endl;
    }
    return 0;
}
