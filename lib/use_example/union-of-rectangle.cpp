#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T, typename X, typename Fe, typename Fa, typename Fm> class LazySegTree
{
private:
  const T et;
  const X ex;
  int num, nb;
  std::vector<T> dat;
  std::vector<X> lazy;
  Fe eval;
  Fa fa;
  Fm fm;
    
public:
  LazySegTree(std::vector<T> &v, T Et, X Ex, Fe valcal, Fa lazyupdate, Fm lazypropagation) : et(Et), ex(Ex), eval(valcal), fa(lazyupdate), fm(lazypropagation)
  {
    int siz = static_cast<int>(v.size());
    for (nb = 0; 1 << nb < siz; ++nb);
    num = 1 << nb;
    dat = std::vector<T> (num << 1, et);
    lazy = std::vector<X> (num, ex);
    for (int i = 0; i < siz; ++i) dat[i + num] = v[i];
    for (int i = num - 1; i > 0; --i) dat[i] = eval(dat[i << 1], dat[(i << 1) + 1]);
  }
  
  LazySegTree(int n, T Et, X Ex, Fe valcal, Fa lazyupdate, Fm lazypropagation) : et(Et), ex(Ex), eval(valcal), fa(lazyupdate), fm(lazypropagation)
  {
    for (nb = 0; 1 << nb < n; ++nb);
    num = 1 << nb;
    dat = std::vector<T> (num << 1, et);
    lazy = std::vector<X> (num, ex);
  }

  //update [left,right)
  void update(int left, int right, X val)
  {
    left = std::max(0, left), right = std::min(num, right);
    if (left >= right) return;
    left += num;
    right += num;
    for (int i = nb; i > 0; --i)
    {
      if (((left >> i) << i) != left) lazyupdate(left >> i);
      if (((right >> i) << i) != right) lazyupdate((right - 1) >> i);
    }
    for (int l = left, r = right; l < r; l >>= 1, r >>= 1)
    {
      if (l & 1) nodeapply(l++, val);
      if (r & 1) nodeapply(--r, val);
    }
    for (int i = 1; i <= nb; ++i)
    {
      if (((left >> i) << i) != left) dat[(left >> i)] = eval(dat[(left >> i) << 1], dat[((left >> i) << 1) + 1]);
      if (((right >> i) << i) != right) dat[(right - 1 >> i)] = eval(dat[(right - 1 >> i) << 1], dat[((right - 1 >> i) << 1) + 1]);
    }
  }
  void update(int id, X val)
  {
    id = std::max(0, id);
    id = std::min(num, id);
    update(id, id + 1, val);
  }

  //get value [left,right)
  T getval(int left, int right)
  {
    left = std::max(0, left), right = std::min(num, right);
    if (left >= right) return et;
    left += num;
    right += num;
    for (int i = nb; i > 0; --i)
    {
      if (((left >> i) << i) != left) lazyupdate(left >> i);
      if (((right >> i) << i) != right) lazyupdate(right - 1 >> i);
    }
    T ansl = et, ansr = et;
    for (int l = left, r = right; l < r; l >>= 1, r >>= 1)
    {
      if (l & 1) ansl = eval(ansl, dat[l++]);
      if (r & 1) ansr = eval(dat[--r], ansr);
    }
    return eval(ansl, ansr);
  }

  //get value [id]
  T getval(int id) {return getval(id, id + 1);}

private:
  void nodeapply(int id, X val)
  {
    dat[id] = fa(dat[id], val);
    if (id < num) lazy[id] = fm(lazy[id], val);
  }
  void lazyupdate(int id)
  {
    nodeapply(id << 1, lazy[id]);
    nodeapply((id << 1) + 1, lazy[id]);
    lazy[id] = ex;
  }
};
template <class T, class X, class Fe, class Fa, class Fm>
LazySegTree(std::vector<T> &, T, X, Fe, Fa, Fm) -> LazySegTree<T, X, Fe, Fa, Fm>;
template <class T, class X, class Fe, class Fa, class Fm>
LazySegTree(int, T, X, Fe, Fa, Fm) -> LazySegTree<T, X, Fe, Fa, Fm>;

long long UnionOfRectangle(vector<pair<pair<int, int>, pair<int, int>>> &p)
{
  int n = p.size();
  vector<int> sx(n), sy(n), tx(n), ty(n), x(2 * n), y(2 * n);
  for (int i = 0; i < n; ++i)
  {
    auto [sxy, txy] = p[i];
    auto [sxx, syy] = sxy;
    auto [txx, tyy] = txy;
    if (sxx > txx) swap(sxx, txx);
    if (syy > tyy) swap(syy, tyy);
    sx[i] = sxx;
    tx[i] = txx;
    sy[i] = syy;
    ty[i] = tyy;
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
  auto fm = [](int a, int b) -> int {return a + b;};
  LazySegTree lsg(v, {1 << 30, 0}, 0, eval, fa, fm);
  long long ans = 0, dif = y.back() - y[0];
  for (int i = 0; i < x.size() - 1; ++i)
  {
    for (auto &v : q[i])
    {
      if (v.first < v.second) lsg.update(v.first, v.second, 1);
      else
      {
        swap(v.first, v.second);
        lsg.update(v.first, v.second, -1);
      }
    }
    auto tmp = lsg.getval(0, y.size() - 1);
    if (tmp.first == 0) ans += (long long)(dif - tmp.second) * (x[i + 1] - x[i]);
    else ans += dif * (x[i + 1] - x[i]);
  }
  return ans;
}