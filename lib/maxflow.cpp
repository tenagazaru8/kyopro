#include <iostream>
#include <queue>
#include <vector>

template<typename T>
class maxflow
{
  struct edge {int to, rep; T cap; edge(int t, int r, T c) : to(t), rep(r), cap(c) {}};
  std::vector<std::vector<edge>> g;
  std::vector<int> dis, id;
public:
  maxflow(int n) : g(n), id(n), dis(n) {}
  void add_edge(int s, int t, T f)
  {
    g[s].push_back({t, (int)g[t].size() + (s == t), f});
    g[t].push_back({s, (int)g[s].size() - 1, 0});
  }
  T flow(int s, int t)
  {
    T ret = 0;
    while (bfs(s, t))
    {
      id.assign(id.size(), 0);
      ret += path(s, t, T((1LL << 60) | (1 << 30)));
    }
    return ret;
  }
private:
  bool bfs(int s, int t)
  {
    dis.assign(g.size(), g.size());
    dis[s] = 0;
    std::queue<int> q;
    for (q.push(s); !q.empty(); q.pop())
    {
      int now = q.front();
      for (auto &v : g[now])
      {
        if (v.cap > 0 && dis[v.to] > dis[now] + 1)
        {
          dis[v.to] = dis[now] + 1;
          q.push(v.to);
        }
      }
    }
    return dis[t] < g.size();
  }
  T path(int s, int t, T lim)
  {
    if (s == t) return lim;
    T now = 0;
    for (int &i = id[t]; i < g[t].size(); ++i)
    {
      auto &e = g[t][i], &re = g[e.to][e.rep];
      if (re.cap <= 0 || dis[e.to] >= dis[t] || dis[e.to] < 0) continue;
      T tmp = path(s, e.to, std::min(lim - now, re.cap));
      if (tmp == 0) continue;
      e.cap += tmp;
      re.cap -= tmp;
      now += tmp;
      if (now >= lim) break;
    }
    return now;
  }
};