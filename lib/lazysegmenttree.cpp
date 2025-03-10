
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