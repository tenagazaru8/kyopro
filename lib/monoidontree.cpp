//製作途中

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T> class monoidontree
{
    int n;
    vector<vector<pair<int, T>>> g;
    vector<T> dub[20];
    bool isbuild;
    T e;
    T (*const eval)(T &, T &) {};

public:
    monoidontree(int num, T E, T (*func)(T &, T &)) : e(E), eval(func), n(num), 
    {
        g.resize(n);
        for (int i = 0; i < 20; ++i) dub[i].resize(n);
    }

    void add_edge(int u, int v, T val)
    {
        g[u].emplace_back(v, val);
    }

    void build()
    {
        if (isbuild) return;
        isbuild = true;

    }

    T getval(int s, int t)
    {

    }
};

void solve(int n, vec)
int main()
{
    int n;
    cin >> n;
    vector<pair<int, intuio9
>> e(n - 1);

}