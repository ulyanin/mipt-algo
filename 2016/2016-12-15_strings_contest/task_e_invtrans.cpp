//
// Created by ulyanin on 15.12.16.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<int> z_by_prefix(const vector<int> &pref)
{
    vector<int> z(pref.size());
    int n = pref.size();
    for (int i = 1; i < n; ++i) {
        if (pref[i] > 0) {
            z[i - pref[i] + 1] = pref[i];
        }
    }
    z[0] = pref.size();
    for (int i = 1, t; i < n; i = t + 1) {
        t = i;
        if (z[i] > 0) {
            for (int j = 1; j < z[i]; ++j) {
                if (z[i + j] > z[j]) {
                    break;
                }
                z[i + j] = min(z[j], z[i] - j);
                t = i + j;
            }
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
    int n;
    cin >> n;
    vector<int> pref(n);
    for (int i = 0; i < n; ++i) {
        cin >> pref[i];
    }
    vector<int> z(z_by_prefix(pref));
    for (int val : z) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
