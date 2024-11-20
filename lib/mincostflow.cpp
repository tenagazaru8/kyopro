#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

template<typename T>
class mincostflow
{
  struct edge {int to, rep; T cap, cost;};
  std::vector<std::vector<edge>> g;
  std::vector<T> h, dis;
  std::vector<int> prevv, preve;
  const T inf = (1LL << 60) | (1LL << 30);
public:
  mincostflow(int n) : g(n), dis(n), prevv(n), preve(n) {}
  void add_edge(int s, int t, T cap, T cost)
  {
    g[s].emplace_back(t, (int)g[t].size(), cap, cost);
    g[t].emplace_back(s, (int)g[s].size() - 1, 0, -cost);
  }
  T min_cost_flow(int s, int t, T f, bool isdag = false)
  {
    h.assign(g.size(), 0);
    if (isdag)
    {
      dis.assign(g.size(), inf);
      std::vector<int> inc(g.size());
      for (int i = 0; i < g.size(); ++i) for (auto &e : g[i]) inc[e.to] += (e.cap > 0);
      std::queue<int> q;
      dis[s] = 0;
      for (int i = 0; i < g.size(); ++i) if (!inc[i]) q.push(i);
      while (!q.empty())
      {
        int now = q.front();
        q.pop();
        for (int i = 0; i < g[now].size(); ++i)
        {
          auto &e = g[now][i];
          if (e.cap == 0) continue;
          if (--inc[e.to] == 0) q.push(e.to);
          if (dis[e.to] > dis[now] + e.cost)
          {
            dis[e.to] = dis[now] + e.cost;
            prevv[e.to] = now;
            preve[e.to] = i;
          }
        }
      }
    }
    else dijkstra(s);
    T ret = 0;
    while (true)
    {
      if (dis[t] == inf) return inf;
      for (int i = 0; i < g.size(); ++i) h[i] += dis[i];
      T d = f;
      for (int now = t; now != s; now = prevv[now]) d = std::min(d, g[prevv[now]][preve[now]].cap);
      f -= d;
      ret += d * h[t];
      for (int now = t; now != s; now = prevv[now])
      {
        g[prevv[now]][preve[now]].cap -= d;
        g[now][g[prevv[now]][preve[now]].rep].cap += d;
      }
      if (f <= 0) return ret;
      dijkstra(s);
    }
    return ret;
  }
private:
  void dijkstra(int s)
  {
    dis.assign(g.size(), inf);
    dis[s] = 0;
    std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> pq;
    pq.emplace(0, s);
    while (!pq.empty())
    {
      auto [d, now] = pq.top();
      pq.pop();
      if (dis[now] < d) continue;
      for (int i = 0; i < g[now].size(); ++i)
      {
        edge &e = g[now][i];
        if (e.cap > 0 && dis[e.to] > dis[now] + e.cost + h[now] - h[e.to])
        {
          dis[e.to] = dis[now] + e.cost + h[now] - h[e.to];
          prevv[e.to] = now;
          preve[e.to] = i;
          pq.emplace(dis[e.to], e.to);
        }
      }
    }
  }
};