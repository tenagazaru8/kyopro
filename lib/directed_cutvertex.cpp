#include <iostream>
#include <vector>
#include <algorithm>

namespace digraphPreprocess
{
    struct edge
    {
        int to, revid, cap;
        edge(int t, int r, int c) : to(t), cap(c), revid(r) {}
    };
    int countv, nowdel;
    double delpernum;
    const int NMAX = 300000;
    std::vector<int> G[NMAX];
    std::vector<edge> g[NMAX * 2];
    int nowpath[NMAX];
    int sorce, sink, n, start, terminal, nowpathsiz;
    int isused[NMAX], ischecked[NMAX * 2], res[NMAX];
    bool iscolored[2 * NMAX];

    bool findincleasepath(const int s, const int t)
    {
        std::vector<int> tmp;
        // static int tmp[2 * NMAX];
        int siz = 0;
        std::stack<int> st;
        st.push(s);
        bool res = false;
        while (!st.empty())
        {
            int now = st.top();
            st.pop();
            if (now == t)
            {
                res = true;
                break;
            }
            if (!ischecked[now]) continue;
            ischecked[now] = 1;
            tmp.push_back(now);
            // tmp[siz++] = now;
            for (auto &[to, ri, cp] : g[now]) if (cp > 0) st.push(to);
        }
        while (!tmp.empty())
        {
            ischecked
        }
        while (siz > 0) ischecked[tmp[--siz]] = 0;
        return res;
    }

    bool findpath(const int s, const int t)
    {
        static int tmp[NMAX];
        int siz = 0;
        std::stack<int> st;
        st.push(s);
        bool res = false;
        while (!st.empty())
        {
            int now = st.top();
            st.pop();
            if (now == t)
            {
                res = true;
                break;
            }
            if (isused[now]) continue;
            isused[now] = 2;
            tmp[siz++] = now;
            for (int to : G[now]) if (!isused[to]) st.push(to);
        }
        while (siz > 0) isused[tmp[--siz]] = 0;
        return res;
    }

    void add_edge(const int s, const int t)
    {
        g[s].emplace_back(t, g[t].size(), 1);
        g[t].emplace_back(s, g[s].size() - 1, 0);
    }

    void dfs2(int s)
    {
        std::stack<int> st;
        st.push(s);
        while (!st.empty())
        {
            int now = st.top();
            st.pop();
            if (iscolored[now]) continue;
            iscolored[now] = true;
            for (auto &[to, ri, cp] : g[now]) if (cp > 0 && !iscolored[to]) st.push(to);
        }
    }

    // void dfs1(int now)
    // {
    //     if (now % 2)
    //     {
    //         ++countv;
    //         int ndpn = 1000 * countv / n;
    //         if (ndpn > delpernum * 10)
    //         {
    //             delpernum = ndpn / 10.0;
    //             fprintf(stderr, "flow-preprocessing: %.1f%\n", delpernum);
    //         }
    //         nowpath[nowpathsiz++] = now / 2;
    //         for (int i = 0; i < (int)g[now].size(); ++i)
    //         {
    //             auto &[to, ri, cp] = g[now][i];
    //             if (cp == 0 || res[to / 2] != -1) continue;
    //             std::swap(cp, g[to][ri].cap);

    //             dfs1(to);

    //             std::swap(cp, g[to][ri].cap);
    //         }
    //         --nowpathsiz;
    //     }
    //     else
    //     {
    //         res[now / 2] = 1;
    //         // flow(now) > 1
    //         if (!findincleasepath(sorce, now))
    //         {
    //             std::vector<int> tmpid;
    //             for (int i = 0; i < 2 * n; ++i) iscolored[i] = 0;
    //             iscolored[sorce - 1] = true;
    //             iscolored[sink + 1] = true;
    //             for (int i = 0; i < nowpathsiz; ++i)
    //             {
    //                 int v = nowpath[i];
    //                 dfs2(2 * v);
    //                 if (iscolored[v * 2] && !iscolored[2 * v + 1])
    //                 {
    //                     isused[v] = 1;
    //                     tmpid.emplace_back(v);
    //                 }
    //                 if (!iscolored[2 * v + 1]) dfs2(2 * v + 1);
    //             }
    //             res[now / 2] = findpath(now / 2, terminal);
    //             while (!tmpid.empty())
    //             {
    //                 isused[tmpid.back()] = 0;
    //                 tmpid.pop_back();
    //             }
    //         }
    //         if (res[now / 2] == 0)
    //         {
    //             for (auto &[to, ri, cp] : g[now])
    //             {
    //                 cp = 0;
    //                 g[to][ri].cap = 0;
    //             }
    //             for (auto &[to, ri, cp] : g[now + 1])
    //             {
    //                 cp = 0;
    //                 g[to][ri].cap = 0;
    //             }
    //             return;
    //         }
    //         auto &[to, ri, cp] = g[now].back();
    //         std::swap(cp, g[to][ri].cap);

    //         dfs1(to);

    //         std::swap(cp, g[to][ri].cap);
    //     }
    // }
    void dfs1(int s)
    {
        std::stack<std::pair<int, int>> st;
        int tmpid[NMAX];
        bool isflow[2 * NMAX];
        for (int i = 0; i < 2 * NMAX; ++i) isflow[i] = false;
        isflow[s] = true;
        isflow[g[s].back().to] = true;
        st.emplace(-s - 1, g[s].size() - 1);
        st.emplace(s, g[s].size() - 1);
        while (!st.empty())
        {
            auto [now, id] = st.top();
            st.pop();
            if (now >= 0)
            {
                std::swap(g[now][id].cap, g[g[now][id].to][g[now][id].revid].cap);
                if (now & 1)
                {
                    ++countv;
                    int ndpn = 1000 * countv / n;
                    if (ndpn > delpernum * 10)
                    {
                        delpernum = ndpn / 10.0;
                        fprintf(stderr, "flow-preprocessing: %.1f%\n", delpernum);
                    }

                    
                    nowpath[nowpathsiz++] = (now >> 1);
                    for (int i = 0; i < (int)g[now].size(); ++i)
                    {
                        auto &[to, ri, cp] = g[now][i];
                        if (cp == 0 || res[to >> 1] != -1 || isflow[to]) continue;
                        isflow[to] = true;
                        st.emplace(-to - 1, ri);
                        st.emplace(to, ri);
                    }
                }
                else
                {
                    res[now >> 1] = 1;
                    // flow(now) > 1
                    if (!findincleasepath(sorce, now))
                    {
                        int tmpidsiz = 0;
                        for (int i = 0; i < (n << 1); ++i) iscolored[i] = 0;
                        iscolored[sorce - 1] = true;
                        iscolored[sink + 1] = true;
                        for (int i = 0; i < nowpathsiz; ++i)
                        {
                            int v = nowpath[i];
                            dfs2(v << 1);
                            if (iscolored[v << 1] && !iscolored[(v << 1) | 1])
                            {
                                isused[v] = 1;
                                tmpid[tmpidsiz++] = v;
                            }
                            if (!iscolored[(v << 1) | 1]) dfs2((v << 1) | 1);
                        }
                        res[now >> 1] = findpath((now >> 1), terminal);
                        while (--tmpidsiz >= 0) isused[tmpid[tmpidsiz]] = 0;
                    }
                    if (res[now >> 1] == 0)
                    {
                        for (auto &[to, ri, cp] : g[now])
                        {
                            cp = 0;
                            g[to][ri].cap = 0;
                        }
                        for (auto &[to, ri, cp] : g[now + 1])
                        {
                            cp = 0;
                            g[to][ri].cap = 0;
                        }
                        continue;
                    }
                    auto &[to, ri, cp] = g[now].back();
                    isflow[to] = true;
                    st.emplace(-to - 1, ri);
                    st.emplace(to, ri);
                }
            }
            else
            {
                now = -now - 1;
                std::swap(g[now][id].cap, g[g[now][id].to][g[now][id].revid].cap);
                if (now & 1) --nowpathsiz;
            }
        }
    }

    std::vector<bool> flowPreprocess(int vertexnum, std::vector<std::pair<int, int>> e, const int startpoint, const int terminalpoint)
    {
        nowdel = 0;
        countv = 0;
        delpernum = 0;
        n = vertexnum;
        start = startpoint;
        terminal = terminalpoint;
        std::sort(e.begin(), e.end());
        e.erase(std::unique(e.begin(), e.end()), e.end());
        for (int i = 0; i < n; ++i)
        {
            isused[i] = 0;
            res[i] = -1;
        }
        for (int i = 0; i < 2 * n; ++i)
        {
            ischecked[i] = 0;
            iscolored[i] = 0;
        }
        for (auto [u, v] : e)
        {
            if (u == v) continue;
            int from = 2 * u + 1, to = 2 * v;
            G[u].emplace_back(v);
            add_edge(from, to);
        }
        for (int i = 0; i < n; ++i)
        {
            if (terminal == i) continue;
            int from = 2 * i, to = 2 * i + 1;
            add_edge(from, to);
        }
        preprocessdbg("        constructed flow-graph")
        sorce = 2 * start + 1, sink = 2 * terminal;
        res[start] = 1;
        res[terminal] = 1;
        dfs1(sorce);
        std::vector<bool> ans(n, false);
        for (int i = 0; i < n; ++i) if (res[i] == 1) ans[i] = true;
        for (int i = 0; i < n; ++i) std::vector<int>().swap(G[i]);
        for (int i = 0; i < 2 * n; ++i) std::vector<edge>().swap(g[i]);
        return ans;
    }
};

class digcut
{
    struct edge
    {
        int to, revid;
        bool cap;
        edge(int t, int r, bool c) : to(t), cap(c), revid(r) {}
    };
    vector<vector<edge>> g;
public:
    digcut(int n) : g(vector<vector<edge>> (2 * n))
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

    vector<int> getcutvertices(int s, int t)
    {
        vector<int> p, dis(2 * n, 1e9), ret;
        queue<int> q;
        dis[2 * t + 1] = 0;
        for (q.emplace(2 * t + 1); !q.empty(); q.pop())
        {
            int now = q.front();
            for (auto &e : g[now])
            {
                if (!e.c && dis[e.to] == 1e9)
                {
                    dis[e.to] = dis[now] + 1;
                    q.emplace(e.to);
                }
            }
        }
        if (dis[2 * s] == 1e9) return ret;
        vector<pair<int, int>> revid;
        int now = 2 * s;
        while (dis[now] > 0)
        {
            for (int i = 0; i < g[now].size(); ++i)
            {
                
                g[now]
                if ()
            }
        }
        for ()
        for ()
    }
}