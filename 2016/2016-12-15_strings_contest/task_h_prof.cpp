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

const int MAXN = 3 * (int)(1e3) + 10;

typedef long long ll;

const ll BASE = 129;
const ll MOD = (ll)(1e9) + 103;

#define disable_modulo

ll mod(ll a, ll b)
{
#ifdef disable_modulo
    return a;
#else
    if (a < 0)
        return (a % b + b) % b;
    return a % b;
#endif
}

using namespace std;

ll Powers[MAXN];

class StringHash
{
public:
    StringHash(const std::string &str = "");
    void calc_hash();
    ll get_hash(ll L, ll R);
private:
    std::string str_;
    std::vector<ll> hash_;
};


StringHash::StringHash(const std::string &str)
        : str_(str)
        , hash_(str.size() + 1)
{}

void StringHash::calc_hash()
{
    Powers[0] = 1;
    hash_[0] = 0;
    for (size_t i = 0; i < str_.size(); ++i) {
#ifndef disable_modulo
        hash_[i + 1] = (hash_[i] * BASE + (str_[i] - 'a' + 1)) % MOD;
        Powers[i + 1] = Powers[i] * BASE % MOD;
#else
        hash_[i + 1] = (hash_[i] * BASE + (str_[i] - 'a' + 1));
        Powers[i + 1] = Powers[i] * BASE;
#endif
    }
}

ll StringHash::get_hash(ll L, ll R)
{
#ifdef disable_modulo
    return hash_[R] - hash_[L] * Powers[R - L];
#else
    return mod(hash_[R] - hash_[L] * Powers[R - L], MOD);
#endif
}


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


char buffer[MAXN];

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    scanf("%s", buffer);
    std::string in_str(buffer);
    StringHash str(in_str);
    str.calc_hash();
    int n = in_str.size();
    unordered_set<ll> hashes;
//    unordered_set<string> strings;
    for (int i = 0; i < n; ++i) {
        string cur(in_str.substr(i));
        vector<int> z(z_function(cur));
        int max_repeted = 0;
        for (int j = i + 1; j < n; ++j) {
            max_repeted = max(max_repeted, min(j - i, z[j - i]));
        }
//        cout << cur << " " << max_repeted << endl;
        for (int j = 1; j <= max_repeted; ++j) {
            hashes.insert(str.get_hash(i, i + j));
//            strings.insert(in_str.substr(i, j));
//            cout << in_str.substr(i, i + j) << endl;
        }
    }
//    cout << endl;
//    for (string s : strings) {
//        cout << s << endl;
//    }
    cout << hashes.size() << endl;
    return 0;
}
