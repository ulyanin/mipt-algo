//
// Created by ulyanin on 15.12.16.

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>

typedef long long ll;

//#define disable_modulo

const ll BASE = 129;
const ll MOD = (ll)(1e9) + 103;
const int MAXN = (int)(1e5) + 10;

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
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int l1, r1, l2, r2;
        scanf("%d%d%d%d", &l1, &r1, &l2, &r2);
        --l1, --l2;
        if (str.get_hash(l1, r1) == str.get_hash(l2, r2)) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
    }
    return 0;
}
