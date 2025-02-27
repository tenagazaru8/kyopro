#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct UF
{
  int gnum;
  vector<int> par, rank, size;
  UF(int n) : par(n + 5), rank(n + 5), size(n + 5, 1), gnum(n) {for (int i = 0; i <= n; ++i) par[i] = i;}
  int root(int x) {return par[x] == x ? x : (par[x] = root(par[x]));}
  void unite(int x, int y)
  {
    x = root(x);
    y = root(y);
    if (x == y) return;
    --gnum;
    if (rank[x] < rank[y]) swap(x, y);
    par[y] = x;
    if (rank[x] == rank[y]) ++rank[x];
    size[x] += size[y];
  }
  int issame(int x, int y) {return root(x) == root(y);}
  int siz(int x) {return size[root(x)];}
  int gsiz() {return gnum;}
};

template<typename T>
struct WUF
{
  vector<int> par, rank, size;
  vector<T> weight;
  WUF(int n, T e = 0) : par(n + 5), rank(n + 5), size(n + 5, 1), weight(n + 5, e) {for (int i = 0; i <= n; ++i) par[i] = i;}
  int root(int x) {if (par[x] == x) {return x;} int r = root(par[x]); weight[x] += weight[par[x]]; return par[x] = r;}
  void unite(int x, int y, T w)
  {
    w += wt(x) - wt(y);
    x = root(x);
    y = root(y);
    if (x == y) return;
    if (rank[x] < rank[y]) {swap(x, y); w = -w;}
    par[y] = x;
    if (rank[x] == rank[y]) ++rank[x];
    size[x] += size[y];
    weight[y] = w;
  }
  int issame(int x, int y) {return root(x) == root(y);}
  int siz(int x) {return size[root(x)];}
  T wt(int x) {root(x); return weight[x];}
  T dif(int x, int y) {return wt(y) - wt(x);}
};

// オイラー路の構築，オイラーグラフを前提として始点と辺集合を与える．
// 適当に改造してるからバグがあるかも．
vector<int> ET(int s, vector<pair<int, int>> &e, bool directed = false)
{
  int n = 0;
  for (auto &v : e) n = max(n, max(v.first, v.second) + 1);
  vector<int> ret;
  if (directed)
  {
    vector<vector<int>> g(n);
    for (auto &v : e) g[v.first].push_back(v.second);
    auto dfs = [&](auto f, int now) -> void
    {
      while (!g[now].empty())
      {
        int v = g[now].back();
        g[now].pop_back();
        f(f, v);
      }
      ret.push_back(now);
    };
    dfs(dfs, s);
  }
  else
  {
    vector<vector<pair<int, int>>> g(n);
    vector<int> id(n);
    for (auto &v : e)
    {
      g[v.first].push_back({v.second, g[v.second].size() + (v.first == v.second)});
      g[v.second].push_back({v.first, g[v.first].size() - 1});
    }
    vector<vector<bool>> used(n);
    for (int i = 0; i < n; ++i) used[i].resize(g[i].size());
    auto dfs = [&](auto f, int now) -> void
    {
      while (id[now] < g[now].size())
      {
        while (id[now] < g[now].size() && used[now][id[now]]) ++id[now];
        if (id[now] >= g[now].size()) break;
        used[now][id[now]] = true;
        int v = g[now][id[now]].first, vid = g[now][id[now]].second;
        used[v][vid] = true;
        f(f, v);
      }
      ret.push_back(now);
    };
    dfs(dfs, s);
  }
  reverse(ret.begin(), ret.end());
  return ret;
}