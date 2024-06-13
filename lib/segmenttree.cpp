#include <iostream>
#include <vector>
#include <queue>
#include <stack>

template <typename T> class BIT
{
private:
  T e;
  int num;
  std::vector<T> dat;
  T (*eval)(T &, T &) {};
public:
  BIT() : e(0), num(0), dat(std::vector<int>(0)), eval([](int &a, int &b) -> int {return a + b;}){}
  BIT(const BIT<T> &bit) : e(bit.e), num(bit.num), dat(bit.dat), eval(bit.eval){}
  BIT(std::vector<T> &v, T E, T (*func)(T &, T &)) : e(E), eval(func)
  {
    num = static_cast<T>(v.size());
    dat = std::vector<T> (num + 1, e);
    for (int i = 0; i < num; ++i) dat[i + 1] = v[i];
    for (int i = 1; i <= num; ++i)
    {
      int j = i + (i & -i);
      if (j <= num) dat[j] = eval(dat[i], dat[j]);
    }
  }
  BIT(int n, T E, T (*func)(T &, T &)) : e(E), eval(func)
  {
    num = n;
    dat = std::vector<T> (num + 1, e);
  }
  //1-index
  void update(int i, T val) {for (; i <= num; i += i & -i) dat[i] = eval(dat[i], val);}
  //[0,right]
  T getval(int right)
  {
    T ans = e;
    for (; right > 0; right &= right - 1) ans = eval(ans, dat[right]);
    return ans;
  }
  int minright(T val)
  {
    if (val <= 0) return 0;
    int ret = 0, tmp = 1;
    for (int i = num; i > 0; i -= i & -i) tmp = i;
    for (; tmp > 0; tmp >>= 1)
    {
      if (ret + tmp <= num && dat[ret + tmp] < val)
      {
        ret += tmp;
        val -= dat[ret];
      }
    }
    return ret + 1;
  }
}; 

template <typename T> class segTree
{
private:
  const T e;
  int num;
  std::vector<T> dat;
  T (*const eval)(T &, T &) {};
public:
  segTree(std::vector<T> &v, T E, T (*func)(T &, T &)) : e(E), eval(func)
  {
    int siz = static_cast<int>(v.size());
    for (num = 1; num < siz; num <<= 1);
    dat = std::vector<T> (2 * num - 1, e);
    for (int i = 0; i < siz; ++i) dat[i + num - 1] = v[i];
    for (int i = num - 2; i >= 0; --i) dat[i] = eval(dat[i * 2 + 1], dat[i * 2 + 2]);
  }
  segTree(int n, T E, T (*func)(T &, T &)) : e(E), eval(func)
  {
    for (num = 1; num < n; num <<= 1);
    dat = std::vector<T> (2 * num - 1, e);
  }
  void update_a(int i, T val)
  {
    for (i += num - 1, dat[i] = val; i != 0;)
    {
      i = (i - 1) / 2;
      dat[i] = eval(dat[i * 2 + 1], dat[i * 2 + 2]);
    }
  }
  void update_r(int i, T val)
  {
    for (i += num - 1, dat[i] = eval(dat[i], val); i != 0;)
    {
      i = (i - 1) / 2;
      dat[i] = eval(dat[i * 2 + 1], dat[i * 2 + 2]);
    }
  }
  T getval(int left, int right)
  {
    left = max(0, left), right = min(num, right);
    T ansl = e, ansr = e;
    for (left += num - 1, right += num - 1; left < right; left >>= 1, right >>= 1)
    {
      if (!(left & 1)) ansl = eval(ansl, dat[left]);
      if (--right & 1) ansr = eval(dat[right], ansr);
    }
    return eval(ansl, ansr);
  }
  T getval(int id) {return dat[num - 1 + id];}
};


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