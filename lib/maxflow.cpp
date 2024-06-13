#include <iostream>
#include <stack>
#include <queue>
#include <vector>

template<typename T>
class maxflow
{
  struct edge {int to, rep; T cap;};
  std::vector<std::vector<edge>> g;
  std::vector<int> dis, id;
public:
  maxflow(int n) : g(n), id(n), dis(n) {}
  void add(int s, int t, T f)
  {
    g[s].push_back({t, g[t].size(), f});
    g[t].push_back({s, g[s].size() - 1, 0});
  }
  T flow(int s, int t)
  {
    T ret = 0;
    while (true)
    {
      bfs(s);
      if (dis[t] <= dis[s]) return ret;
      id.assign(id.size(), 0);
      T val;
      while ((val = path(s, t)) > 0) ret += val;
    }
  }
private:
  void bfs(int s)
  {
    dis.assign(g.size(), -1);
    dis[s] = 0;
    std::queue<int> q;
    for (q.push(s); !q.empty(); q.pop())
    {
      int now = q.front();
      for (auto &v : g[now])
      {
        if (v.cap > 0 && dis[v.to] < 0)
        {
          dis[v.to] = dis[now] + 1;
          q.push(v.to);
        }
      }
    }
  }
  T path(int s, int t)
  {
    const T inf = ((1LL << 60) | (1 << 30));
    std::stack<edge> st;
    T ret = inf;
    st.push({s, -1, inf});
    while (!st.empty())
    {
      auto now = st.top();
      st.pop();
      if (now.rep == -1)
      {
        if (now.to == t)
        {
          ret = now.cap;
          break;
        }
        for (int &i = id[now.to]; i < g[now.to].size(); ++i)
        {
          auto &e = g[now.to][i];
          if (dis[e.to] > dis[now.to] && e.cap > 0)
          {
            st.push({e.to, e.rep, 0});
            st.push({e.to, -1, std::min(e.cap, now.cap)});
          }
        }
      }
    }
    if (ret == inf) return 0;
    int now = t;
    while (now != s)
    {
      while (!st.empty() && st.top().to != now) st.pop();
      edge &v1 = st.top(), &v2 = g[v1.to][v1.rep];
      v2.cap += ret;
      g[v2.to][v2.rep].cap -= ret;
      now = v2.to;
    }
    return ret;
  }
};



int main()
{
}