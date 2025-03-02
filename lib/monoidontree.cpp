//製作途中

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

template <typename T> class monoidontree
{
    int n, bit;
    vector<vector<pair<int, T>>> g;
    vector<T> dubu[20], dubd[20];
    vector<int> dub[20], dep;
    bool isbuild;
    T e;
    T (*const eval)(T &, T &) {};

public:
    monoidontree(int num, T E, T (*func)(T &, T &)) : e(E), eval(func), n(num), 
    {
        g.resize(n);
        dep.assign(n, n + 1);
        for (bit = 0; (1 << bit) < n; ++bit);
        ++bit;
        for (int i = 0; i < bit; ++i)
        {
            dub[i].assign(n, -1);
            dubu[i].assign(n, e);
            dubd[i].assign(n, e);
        }
    }

    void add_edge(int u, int v, T val) {g[u].emplace_back(v, val);}

    void build()
    {
        if (isbuild) return;
        isbuild = true;
        vector<int> st;
        dub[0] = -1;
        dep[0] = 0;
        for (st.push_back(0); !st.empty();)
        {
            int now = st.back();
            st.pop_back();
            for (auto [to, v] : g[now])
            {
                if (dep[to] > dep[now] + 1)
                {
                    dep[to] = dep[now] + 1;
                    dub[0][to] = now;
                    dubd[0][to] = v;
                    st.push_back(to);
                }
            }
        }
        for (int i = 0; i < n; ++i)
        {
            for (auto [to, v] : g[i])
            {
                if (to == dub[0][i])
                {
                    dubu[0][i] = v;
                    break;
                }
            }
        }
        for (int i = 1; i < bit; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (dub[i - 1][j] == -1) continue;
                dub[i][j] = dub[i - 1][dub[i - 1][j]];
                if (dub[i][j] != -1)
                {
                    dubu[i][j] = eval(dubu[i - 1][j], dubu[i - 1][dub[i - 1][j]]);
                    dubd[i][j] = eval(dubd[i - 1][dub[i - 1][j]], dubd[i - 1][j]);
                }
            }
        }
    }

    T getval(int s, int t)
    {
        if (!isbuild) exit(2);
        int ds = dep[s], dt = dep[t];
        T ansu = e, ansd = e;
        if (ds < dt)
        {
            int d = ds - dt;
            for (int i = 0; i < bit && d > 0; ++i)
            {
                if (d & (1 << i))
                {
                    ansu = eval(ansu, dubu[i][s]);
                    s = dub[i][s];
                    d ^= (1 << i);
                }
            }
        }
        else if (ds > dt)
        {
            int d = dt - ds;
            for (int i = 0; i < bit && d > 0; ++i)
            {
                if (d & (1 << i))
                {
                    ansd = eval(dubd[i][t], ansd);
                    t = dub[i][t];
                    d ^= (1 << i);
                }
            }
        }
        
    }
};

int main()
{

}