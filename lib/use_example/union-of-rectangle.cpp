#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// 長方形区画の和集合の面積を求める。
// O(NlogN)

template <typename T, typename X> class LazySegTree
{
private:
  const T et;
  const X ex;
  int num;
  std::vector<T> dat;
  std::vector<X> lazy;
  T (*const eval)(const T, const T) {};
  T (*const fa)(const T, const X) {};
  X (*const fm)(const X, const X) {};
  X (*const fp)(const X, int) {};
    
public:
  LazySegTree(std::vector<T> &v, T Et, X Ex, T (*valcal)(T, T), T (*lazyupdate)(T, X), X (*lazypropagation)(X, X), X (*rangelazy)(X, int)) : et(Et), ex(Ex), eval(valcal), fa(lazyupdate), fm(lazypropagation), fp(rangelazy)
  {
    int siz = static_cast<int>(v.size());
    for (num = 1; num < siz; num <<= 1);
    dat = std::vector<T> (2 * num - 1, et);
    lazy = std::vector<X> (2 * num - 1, ex);
    for (int i = 0; i < siz; ++i) dat[i + num - 1] = v[i];
    for (int i = num - 2; i >= 0; --i) dat[i] = eval(dat[i * 2 + 1], dat[i * 2 + 2]);
  }

  LazySegTree(int n, T Et, X Ex, T (*valcal)(T, T), T (*lazyupdate)(T, X), X (*lazypropagation)(X, X), X (*rangelazy)(X, int)) : et(Et), ex(Ex), eval(valcal), fa(lazyupdate), fm(lazypropagation), fp(rangelazy)
  {
    for (num = 1; num < n; num <<= 1);
    dat = std::vector<T> (2 * num - 1, et);
    lazy = std::vector<X> (2 * num - 1, ex);
  }

  //update [left,right)
  void rangeupdate(int left, int right, X val)
  {
    std::stack<std::pair<int, int>> st;
    std::stack<int> s;
    for (st.push({0, num}); !st.empty();)
    {
      auto v = st.top();
      st.pop();
      int now = (num + v.first) / (v.second - v.first) - 1;
      lazyupdate(now, v.second - v.first);
      if (left <= v.first && v.second <= right)
      {
        lazy[now] = fm(lazy[now], val);
        lazyupdate(now, v.second - v.first);
      }
      else if (!(v.second <= left || right <= v.first))
      {
        st.push({(v.first + v.second) / 2, v.second});
        st.push({v.first, (v.first + v.second) / 2});
        s.push(now);
      }
    }
    while (!s.empty())
    {
      int index = s.top();
      dat[index] = eval(dat[2 * index + 1], dat[2 * index + 2]);
      s.pop();
    }
  }

  //get value [left,right)
  T getval(int left, int right)
  {
    std::stack<std::pair<int, int>> s;
    T ans = et;
    for (s.push({0, num}); !s.empty();)
    {
      auto v = s.top();
      s.pop();
      int now = (num + v.first) / (v.second - v.first) - 1;
      lazyupdate(now, v.second - v.first);
      if (left <= v.first && v.second <= right) ans = eval(ans, dat[now]);
      else if (!(v.second <= left || right <= v.first))
      {
        s.push({(v.first + v.second) / 2, v.second});
        s.push({v.first, (v.first + v.second) / 2});
      }
    }
    return ans;
  }

  //get value [id]
  T getval(int id) {return getval(id, id + 1);}

private:
  void lazyupdate(int index, int len)
  {
    if (lazy[index] == ex) return;
    if (index < num - 1)
    {
      lazy[index * 2 + 1] = fm(lazy[index * 2 + 1], lazy[index]);
      lazy[index * 2 + 2] = fm(lazy[index * 2 + 2], lazy[index]);
    }
    dat[index] = fa(dat[index], fp(lazy[index], len));
    lazy[index] = ex;
  }
};

int main()
{
  int n;
  cin >> n;
  vector<int> sx(n), sy(n), tx(n), ty(n), x(2 * n), y(2 * n);
  for (int i = 0; i < n; ++i)
  {
    cin >> sx[i] >> sy[i] >> tx[i] >> ty[i];
    x[i * 2] = sx[i], x[i * 2 + 1] = tx[i];
    y[i * 2] = sy[i], y[i * 2 + 1] = ty[i];
  }
  sort(x.begin(), x.end());
  x.erase(unique(x.begin(), x.end()), x.end());
  sort(y.begin(), y.end());
  y.erase(unique(y.begin(), y.end()), y.end());
  int siz = y.size();
  vector<vector<pair<int, int>>> q(x.size());
  for (int i = 0; i < n; ++i)
  {
    int idx1 = lower_bound(x.begin(), x.end(), sx[i]) - x.begin(), idx2 = lower_bound(x.begin(), x.end(), tx[i]) - x.begin();
    int idy1 = lower_bound(y.begin(), y.end(), sy[i]) - y.begin(), idy2 = lower_bound(y.begin(), y.end(), ty[i]) - y.begin();
    q[idx1].push_back({idy1, idy2});
    q[idx2].push_back({idy2, idy1});
  }
  vector<pair<int, int>> v(y.size() - 1);
  for (int i = 0; i < y.size() - 1; ++i) v[i].second = y[i + 1] - y[i];
  auto eval = [](pair<int, int> a, pair<int, int> b) -> pair<int, int>
  {
    pair<int, int> ret;
    ret.first = min(a.first, b.first);
    if (ret.first == a.first) ret.second += a.second;
    if (ret.first == b.first) ret.second += b.second;
    return ret;
  };
  auto fa = [](pair<int, int> a, int b) -> pair<int, int>
  {
    pair<int, int> ret = a;
    ret.first += b;
    return ret;
  };
  auto fm = [](int a, int b) -> int
  {
    return a + b;
  };
  auto fp = [](int a, int b) -> int
  {
    return a;
  };
  LazySegTree<pair<int, int>, int> lsg(v, {1e9, 0}, 0, eval, fa, fm, fp);
  long long ans = 0, dif = y.back() - y[0];
  for (int i = 0; i < x.size() - 1; ++i)
  {
    for (auto &v : q[i])
    {
      if (v.first < v.second)
      {
        lsg.rangeupdate(v.first, v.second, 1);
      }
      else
      {
        swap(v.first, v.second);
        lsg.rangeupdate(v.first, v.second, -1);
      }
    }
    auto tmp = lsg.getval(0, y.size() - 1);
    if (tmp.first == 0) ans += (long long)(dif - tmp.second) * (x[i + 1] - x[i]);
    else ans += dif * (x[i + 1] - x[i]);
  }
  cout << ans << endl;
}
