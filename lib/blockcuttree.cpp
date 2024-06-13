#include <iostream>
#include <queue>
#include <vector>

using namespace std;

/*
 * block-cut-tree class
 * グラフ上の2点s,tを決めたとき，O(N+M)で全頂点に対して以下の答えを求める．
 * s-tパスであって，頂点vを経由するものが存在するかどうか？
 */
class blockCutTree
{
  const int inf = 1 << 30;
  int n, gnum; // 頂点数，グループの数
  bool isdirected;
  vector<int> eg, dep, low; // 辺のグループ番号，dfs-treeにおける各頂点の深さとlowpoint
  vector<int> gr, apid; // 所属する成分の番号, 関節点かどうか(関節点なら成分の番号,そうでないなら-1)
  vector<vector<pair<int, int>>> g; // グラフ, 辺に持たせる情報は(行先, 辺の番号)
  vector<vector<int>> vgr, egr, bct, bctr;
public:
  // flag=true/false -> directed/underected
  blockCutTree(int num, const vector<pair<int, int>> &e, bool direction = false) :
    n(num), isdirected(direction), g(num), eg(e.size(), -1), dep(num, -1), low(num, inf), gr(num, -1), apid(num, -1)
  {
    int siz = e.size();
    for (int i = 0; i < siz; ++i)
    {
      g[e[i].first].push_back({e[i].second, i});
      // 無向グラフなら逆辺を張る
      if (!isdirected) g[e[i].second].push_back({e[i].first, i});
    }

    vector<int> topo = topologicalsort();
    gnum = 0;
    for (int i : topo)
    {
      if (dep[i] == -1)
      {
        dep[i] = 0;
        low[i] = 0;
        dfs1(i, -1);
        bool isisolation = true;
        for (auto v : g[i])
        {
          if (dep[v.first] == 1 && eg[v.second] == -1)
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
    egr.resize(gnum);
    vgr.resize(gnum + apcnt);
    bct.resize(gnum + apcnt);
    if (isdirected) bctr.resize(gnum + apcnt);
    for (int i = 0; i < n; ++i) if (apid[i] != -1) vgr[apid[i]].push_back(i);
    for (int i = 0; i < siz; ++i) if (eg[i] != -1) egr[eg[i]].push_back(i);
    for (int i = 0; i < gnum; ++i)
    {
      for (int id : egr[i])
      {
        if (gr[e[id].first] != i)
        {
          vgr[i].push_back(e[id].first);
          gr[e[id].first] = i;
          if (apid[e[id].first] != -1)
          {
            int idx = apid[e[id].first];
            bct[idx].push_back(i);
            if (isdirected) bctr[i].push_back(idx);
            else bct[i].push_back(idx);
          }
        }
        if (gr[e[id].second] != i)
        {
          vgr[i].push_back(e[id].second);
          gr[e[id].second] = i;
          if (apid[e[id].second] != -1)
          {
            int idx = apid[e[id].second];
            bct[i].push_back(idx);
            if (isdirected) bctr[idx].push_back(i);
            else bct[idx].push_back(i);
          }
        }
      }
    }
    gnum += apcnt;
  }

  void printparam()
  {
    for (int i = 0; i < n; ++i) cout << apid[i] << " \n"[i == n -1];
    for (int i = 0; i < n; ++i) cout << dep[i] << " \n"[i == n -1];
    for (int i = 0; i < n; ++i) cout << low[i] << " \n"[i == n -1];
    for (int i = 0; i < vgr.size(); ++i)
    {
      cout << "groupnumber:" << i << endl;
      cout << "vertices:";
      for (auto v : vgr[i]) cout << ' ' << v;
      cout << endl;
      cout << "edge->";
      for (auto v : bct[i]) cout << ' ' << v;
      cout << endl << endl;
    }
  }

  // パスの始点sと終点t(0-index)を渡すと，各頂点についてパスに含まれることがあるかを表すbool配列を返す．
  // 0:含まれない，1:含まれる
  vector<bool> isincluded(int s, int t)
  {
    vector<bool> ret(n);
    s = (apid[s] == -1 ? gr[s] : apid[s]);
    t = (apid[t] == -1 ? gr[t] : apid[t]);
    vector<int> sdis(gnum, inf), tdis(gnum, inf);
    sdis[s] = 0;
    queue<int> q;
    for (q.push(s); !q.empty(); q.pop())
    {
      int now = q.front();
      for (int v : bct[now])
      {
        if (sdis[v] > sdis[now] + 1)
        {
          sdis[v] = sdis[now] + 1;
          q.push(v);
        }
      }
    }
    vector<vector<int>> &tree = (isdirected ? bctr : bct);
    tdis[t] = 0;
    for (q.push(t); !q.empty(); q.pop())
    {
      int now = q.front();
      for (int v : tree[now])
      {
        if (tdis[v] > tdis[now] + 1)
        {
          tdis[v] = tdis[now] + 1;
          q.push(v);
        }
      }
    }
    if (sdis[t] < inf) for (int i = 0; i < gnum; ++i) if (sdis[i] + tdis[i] == sdis[t]) for (int v : vgr[i]) ret[v] = true;
    return ret;
  }

private:
  void dfs1(int now, int p)
  {
    int reslow = low[now];
    for (auto v : g[now])
    {
      if (dep[v.first] == -1 && v.first != p)
      {
        dep[v.first] = dep[now] + 1;
        low[v.first] = dep[v.first];
        dfs1(v.first, now);
      }
      if (v.first != p) reslow = min(reslow, low[v.first]);
    }
    low[now] = reslow;
    for (auto v : g[now])
    {
      if (dep[v.first] == dep[now] + 1 && v.first != p && dep[now] <= low[v.first])
      {
        apid[now] = 0;
        eg[v.second] = gnum;
        dfs2(v.first);
        ++gnum;
      }
    }
  }

  void dfs2(int now)
  {
    for (auto v : g[now])
    {
      if (dep[v.first] == dep[now] + 1 && eg[v.second] == -1)
      {
        eg[v.second] = gnum;
        dfs2(v.first);
      }
    }
  }

  vector<int> topologicalsort()
  {
    int idx = n;
    vector<int> ret(n);
    vector<bool> used(n);
    auto topodfs = [&](auto f, int now) -> void
    {
      if (used[now]) return;
      used[now] = true;
      for (auto v : g[now]) f(f, v.first);
      ret[--idx] = now;
    };
    for (int i = 0; i < n; ++i) topodfs(topodfs, i);
    return ret;
  }
};

int main()
{
  int n, m, a, b, c;
  cin >> n >> m >> a >> b >> c;
  --a, --b, --c;
  vector<pair<int, int>> e(m);
  for (int i = 0; i < m; ++i)
  {
    int u, v;
    cin >> u >> v;
    --u, --v;
    e[i] = {u, v};
  }
  blockCutTree bc(n, e);
  vector<bool> res = bc.isincluded(a, c);
  cout << (res[b] ? "Yes" : "No") << endl;
}