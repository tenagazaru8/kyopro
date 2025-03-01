#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

//有向グラフのs-tウォークの最長共通部分列 O(|V|+|E|)
class digcut
{
    struct edge
    {
        int to, revid;
        bool cap;
        edge(int t, int r, bool c) : to(t), cap(c), revid(r) {}
    };
    std::vector<std::vector<edge>> g;
public:
    digcut(int n) : g(std::vector<std::vector<edge>> (2 * n))
    {
        for (int i = 0; i < n; ++i)
        {
            g[2 * i].emplace_back(2 * i + 1, (int)g[2 * i + 1].size(), true);
            g[2 * i + 1].emplace_back(2 * i, (int)g[2 * i].size(), false);
        }
    }

    void add_edge(int u, int v)
    {
        g[2 * u + 1].emplace_back(2 * v, (int)g[2 * v].size(), true);
        g[2 * v].emplace_back(2 * u + 1, (int)g[2 * u + 1].size(), false);
    }

    std::vector<int> getcutvertices(int s, int t)
    {
        std::vector<int> p, dis(g.size(), 1e9), ret;
        std::queue<int> q;
        dis[2 * t + 1] = 0;
        for (q.emplace(2 * t + 1); !q.empty(); q.pop())
        {
            int now = q.front();
            for (auto &e : g[now])
            {
                if (!e.cap && dis[e.to] == 1e9)
                {
                    dis[e.to] = dis[now] + 1;
                    q.emplace(e.to);
                }
            }
        }
        if (dis[2 * s] == 1e9) return ret;
        std::vector<std::pair<int, int>> revid;
        int now = 2 * s;
        while (dis[now] > 0)
        {
            for (int i = 0; i < g[now].size(); ++i)
            {
                auto &e = g[now][i];
                if (e.cap && dis[e.to] < dis[now])
                {
                    revid.emplace_back(now, i);
                    now = e.to;
                    break;
                }
            }
        }
        for (auto [vid, eid] : revid) std::swap(g[vid][eid].cap, g[g[vid][eid].to][g[vid][eid].revid].cap);
        std::vector<bool> used(g.size());
        for (auto [vid, eid] : revid)
        {
            if (!used[vid])
            {
                std::vector<int> st;
                used[vid] = true;
                st.push_back(vid);
                while (!st.empty())
                {
                    int now = st.back();
                    st.pop_back();
                    for (auto &e : g[now])
                    {
                        if (e.cap && !used[e.to])
                        {
                            used[e.to] = true;
                            st.push_back(e.to);
                        }
                    }
                }
            }
            if ((vid & 1) == 0 && !used[g[vid][eid].to]) ret.push_back(vid >> 1);
        }
        for (auto [vid, eid] : revid) std::swap(g[vid][eid].cap, g[g[vid][eid].to][g[vid][eid].revid].cap);
        return ret;
    }
};