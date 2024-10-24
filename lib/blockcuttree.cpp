#include <iostream>
#include <vector>
#include <algorithm>

/*
 * ViaVertexDetermine class
 * 静的な無向グラフが与えられたとき，次のクエリを高速に処理する．
 * query(s, v, t) : 単純 s-t パスであって，頂点 v を経由するものが存在するかどうか？
 * 時間計算量: 前処理 O((|V|+|E|)log(|V|+|E|)), クエリ当たり O(1)
 */
class ViaVertexDetermine
{
private:
    std::vector<int> eg, dep, low, gr, apid, inid, dat, lookup;
    std::vector<std::vector<std::pair<int, int>>> g;
    std::vector<std::vector<int>> egr, bct, sp;
    int gnum, nowdep, nowidx;

public:
    ViaVertexDetermine(std::vector<std::pair<int, int>> e)
    {
        const int inf = 1 << 30;
        {
            int delnum = 0;
            for (int i = 0; i < e.size(); ++i)
            {
                e[i - delnum] = e[i];
                if (e[i].first == e[i].second) ++delnum;
            }
            while (delnum--) e.pop_back();
            for (auto &[u, v] : e) if (u > v) std::swap(u, v);
            std::sort(e.begin(), e.end());
            e.erase(std::unique(e.begin(), e.end()), e.end());
        }
        int n = 0;
        for (auto [u, v] : e) if (n < v) n = v;
        ++n;
        int siz = e.size();
        eg.assign(siz, -1);
        dep.assign(n, -1);
        low.assign(n, inf);
        gr.assign(n, -1);
        apid.assign(n, -1);
        g.resize(n);
        for (int i = 0; i < siz; ++i)
        {
            g[e[i].first].emplace_back(e[i].second, i);
            g[e[i].second].emplace_back(e[i].first, i);
        }
        gnum = 0;
        for (int i = 0; i < n; ++i)
        {
            if (dep[i] == -1)
            {
                dep[i] = 0;
                low[i] = 0;
                dfs1(i, -1);
                bool isisolation = true;
                for (auto [to, id] : g[i])
                {
                    if (dep[to] == 1 && eg[id] == -1)
                    {
                        isisolation = false;
                        break;
                    }
                }
                if (!isisolation)
                {
                    dfs2(i);
                    ++gnum;
                }
            }
        }
        int apcnt = 0;
        for (int i = 0; i < n; ++i)
        {
            if (apid[i] != -1)
            {
                apid[i] = gnum + apcnt;
                ++apcnt;
            }
        }
        egr.assign(gnum, std::vector<int>());
        bct.assign(gnum + apcnt, std::vector<int>());
        for (int i = 0; i < siz; ++i) if (eg[i] != -1) egr[eg[i]].emplace_back(i);
        for (int i = 0; i < gnum; ++i)
        {
            for (int id : egr[i])
            {
                if (gr[e[id].first] != i)
                {
                    gr[e[id].first] = i;
                    if (apid[e[id].first] != -1)
                    {
                        int idx = apid[e[id].first];
                        bct[idx].emplace_back(i);
                        bct[i].emplace_back(idx);
                    }
                }
                if (gr[e[id].second] != i)
                {
                    gr[e[id].second] = i;
                    if (apid[e[id].second] != -1)
                    {
                        int idx = apid[e[id].second];
                        bct[i].emplace_back(idx);
                        bct[idx].emplace_back(i);
                    }
                }
            }
        }
        gnum += apcnt;

        nowdep = 0;
        nowidx = 0;
        inid.assign(gnum, -1);
        dat.assign(2 * gnum, 1 << 30);
        for (int i = 0; i < gnum; ++i)
        {
            if (inid[i] == -1)
            {
                buildfs(i, -1);
                dat[nowidx++] = -inf;
            }
        }
        lookup.resize(2 * gnum);
        lookup[0] = -1;
        for (int i = 1; i < 2 * gnum; ++i) lookup[i] = lookup[i >> 1] + 1;
        int lumax = lookup[2 * gnum - 1];
        sp.resize(lumax + 1);
        sp[0] = dat;
        for (int i = 1; i <= lumax; ++i)
        {
            int siz = 2 * gnum - (1 << i) + 1;
            sp[i].resize(siz);
            for (int j = 0; j < siz; ++j) sp[i][j] = std::min(sp[i - 1][j], sp[i - 1][j + (1 << i - 1)]);
        }
    }

    bool query(int s, int v, int t)
    {
        if (s == t) return (s == v);
        int tmp = 0;
        if (apid[v] != -1) tmp = 2;
        int sid = calidx(s), vid = calidx(v), tid = calidx(t);
        int d1 = calcdis(sid, tid), d2 = calcdis(sid, vid), d3 = calcdis(vid, tid);
        if (d1 == -1 || d2 == -1 || d3 == -1) return false;
        return d1 + tmp >= d2 + d3;
    }

private:

    int calidx(int id) {return apid[id] == -1 ? gr[id] : apid[id];}

    void dfs1(int now, int p)
    {
        int reslow = low[now];
        for (auto [to, id] : g[now])
        {
            if (dep[to] == -1 && to != p)
            {
                dep[to] = dep[now] + 1;
                low[to] = dep[to];
                dfs1(to, now);
            }
            if (to != p) reslow = std::min(reslow, low[to]);
        }
        low[now] = reslow;
        for (auto [to, id] : g[now])
        {
            if (dep[to] == dep[now] + 1 && to != p && dep[now] <= low[to])
            {
                apid[now] = 0;
                eg[id] = gnum;
                dfs2(to);
                ++gnum;
            }
        }
    }

    void dfs2(int now)
    {
        for (auto [to, id] : g[now])
        {
            if (dep[to] == dep[now] + 1 && eg[id] == -1)
            {
                eg[id] = gnum;
                dfs2(to);
            }
        }
    }

    void buildfs(int now, int p)
    {
        if (inid[now] == -1) inid[now] = nowidx;
        dat[nowidx++] = nowdep;
        for (int to : bct[now])
        {
            if (to == p) continue;
            ++nowdep;
            buildfs(to, now);
            --nowdep;
            dat[nowidx++] = nowdep;
        }
    }

    int getval(int l, int r)
    {
        int lu = lookup[r - l + 1];
        return std::min(sp[lu][l], sp[lu][r - (1 << lu) + 1]);
    }

    int calcdis(int s, int t)
    {
        int id1 = inid[s], id2 = inid[t];
        if (id1 > id2) std::swap(id1, id2);
        int mid = getval(id1, id2);
        if (mid < 0) return -1;
        return dat[id1] + dat[id2] - 2 * mid;
    }
};