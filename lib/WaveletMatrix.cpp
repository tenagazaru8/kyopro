#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

template<typename T>
struct WaveletMatrix
{
  struct bv
  {
    vector<uint64_t> v;
    vector<int> sum;
    bv(int siz = 0)
    {
      v.resize((siz + 63) >> 6);
      sum.resize(v.size() + 1);
    }
    void resize(int siz)
    {
      v.resize((siz + 63) >> 6);
      sum.resize(v.size() + 1);
    }
    void flip(int id) {v[id >> 6] ^= (1ULL << (id & 63));}
    bool get(int id) {return (v[id >> 6] >> (id & 63)) & 1;}
    void build() {for (int i = 0; i < v.size(); ++i) sum[i + 1] = sum[i] + __builtin_popcountll(v[i]);}
    int getsum(int id) {return sum[id >> 6] + __builtin_popcountll(v[id >> 6] & ((1ULL << (id & 63)) - 1));}
  };
  int h, w;
  T base = 0;
  vector<bv> mat;
  WaveletMatrix(vector<T> a)
  {
    w = a.size();
    for (int i = 0; i < w; ++i) base = min(base, a[i]);
    h = 0;
    for (int i = 0; i < w; ++i)
    {
      a[i] -= base;
      T tmp = a[i];
      int bc = 0;
      while (tmp > 0)
      {
        ++bc;
        tmp >>= 1;
      }
      h = max(h, bc);
    }
    mat.assign(h, bv(w));
    vector<T> l(w), r(w);
    for (int i = h - 1; i >= 0; --i)
    {
      int lc = 0, rc = 0;
      for (int j = 0; j < w; ++j)
      {
        if ((a[j] >> i) & 1)
        {
          r[rc++] = a[j];
          mat[i].flip(j);
        }
        else l[lc++] = a[j];
      }
      for (int j = 0; j < lc; ++j) a[j] = l[j];
      for (int j = 0; j < rc; ++j) a[j + lc] = r[j];
      mat[i].build();
    }
  }
  pair<pair<int, int>, pair<int, int>> subtree(int x, int l, int r)
  {
    int sl = mat[x].getsum(l), sr = mat[x].getsum(r);
    int l0 = l - sl, r0 = r - sr;
    int c0 = w - mat[x].sum.back();
    int l1 = c0 + sl, r1 = c0 + sr;
    return {{l0, r0}, {l1, r1}};
  }
  T sub_kth_smallest(int x, int l, int r, int k)
  {
    if (x < 0) return 0;
    auto [lr0, lr1] = subtree(x, l, r);
    auto [l0, r0] = lr0;
    auto [l1, r1] = lr1;
    int tmp = r0 - l0;
    if (tmp <= k) return (1ULL << x) + sub_kth_smallest(x - 1, l1, r1, k - tmp);
    else return sub_kth_smallest(x - 1, l0, r0, k);
  }
  T kth_smallest(int l, int r, int k)
  {
    T res = sub_kth_smallest(h - 1, l, r, k);
    res += base;
    return res;
  }
  
  T sub_lower_bound(int x, int l, int r, T v)
  {
    if (x < 0) return 0;
    auto [lr0, lr1] = subtree(x, l, r);
    auto [l0, r0] = lr0;
    auto [l1, r1] = lr1;
    if (v == -1)
    {
      if (r0 - l0 > 0) return sub_lower_bound(x - 1, l0, r0, -1);
      else if (r1 - l1 > 0) return (1ULL << x) + sub_lower_bound(x - 1, l1, r1, -1);
    }
    if ((v >> x) & 1)
    {
      if (r1 - l1 > 0) return sub_lower_bound(x - 1, l1, r1, v - (1ULL << x));
      else return -1;
    }
    else
    {
      if (r0 - l0 > 0)
      {
        T tmp = sub_lower_bound(x - 1, l0, r0, v);
        if (tmp == -1 && r1 - l1 > 0) return (1ULL << x) + sub_lower_bound(x - 1, l1, r1, -1);
        else if (tmp == -1) return -1;
        else return tmp;
      }
      else
      {
        if (r1 - l1 > 0) return (1ULL << x) + sub_lower_bound(x - 1, l1, r1, -1);
        else return -1;
      }
    }
  }
  
  T lower_bound(int l, int r, T x)
  {
    x -= base;
    T res = sub_lower_bound(h - 1, l, r, x);
    return res + base;
  }
};

int main()
{
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; ++i) cin >> a[i];
  WaveletMatrix wm(a);
  for (int i = 0; i <= 10; ++i)
  {
    auto res = wm.lower_bound(0, n, i);
    cout << i << " <= " << res << endl;
  }
  // vector<int> lo(1, 1), up(1, n + 1), le(1, 0), ri(1, n);
  // int q;
  // cin >> q;
  // while (q--)
  // {
  //   int t, s, x;
  //   cin >> t >> s >> x;
  //   if (t == 1)
  //   {
  //     int nlo = lo[s], nup = up[s], nle = le[s], nri = ri[s];

  //   }
  // }
}