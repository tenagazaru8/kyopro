#include <iostream>
#include <vector>

template <typename T, typename F> class SegTree
{
private:
  const T e;
  int num;
  std::vector<T> dat;
  F eval;
public:
  SegTree(std::vector<T> &v, T E, F func) : e(E), eval(func)
  {
    int siz = static_cast<int>(v.size());
    for (num = 1; num < siz; num <<= 1);
    dat = std::vector<T> (2 * num - 1, e);
    for (int i = 0; i < siz; ++i) dat[i + num - 1] = v[i];
    for (int i = num - 2; i >= 0; --i) dat[i] = eval(dat[i * 2 + 1], dat[i * 2 + 2]);
  }
  SegTree(int n, T E, F func) : e(E), eval(func)
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
    left = std::max(0, left), right = std::min(num, right);
    T ansl = e, ansr = e;
    for (left += num - 1, right += num - 1; left < right; left >>= 1, right >>= 1)
    {
      if (!(left & 1)) ansl = eval(ansl, dat[left]);
      if (--right & 1) ansr = eval(dat[right], ansr);
    }
    return eval(ansl, ansr);
  }
  T getval(int id) {return dat[num - 1 + id];}
  template<typename Fc>
  int maxright(int left, Fc check)
  {
    T now = e;
    int id = left + num - 1;
    while (true)
    {
      T tmp = eval(now, dat[id]);
      if (check(tmp))
      {
        if (id & 1) ++id;
        else
        {
          int tmpid = id + 2;
          if ((tmpid & -tmpid) == tmpid) return num;
          id >>= 1;
        }
        now = tmp;
      }
      else
      {
        if (num - 1 <= id && id < num * 2 - 1) break;
        id = (id << 1) + 1;
      }
    }
    return id - num + 1;
  }
  template<typename Fc>
  int minleft(int right, Fc check)
  {
    if (right == 0) return 0;
    T now = e;
    int id = right + num - 2;
    while (true)
    {
      T tmp = eval(dat[id], now);
      if (check(tmp))
      {
        if (id & 1)
        {
          int tmpid = id + 1;
          if ((tmpid & -tmpid) == tmpid) return 0;
          id = (id >> 1);
        }
        --id;
        now = tmp;
      }
      else
      {
        if (num - 1 <= id && id < num * 2 - 1) break;
        id = (id << 1) + 2;
      }
    }
    return id - num + 2;
  }
};
template <class T, class F>
SegTree(std::vector<T> &, T, F) -> SegTree<T, F>;
template <class T, class F>
SegTree(int, T, F) -> SegTree<T, F>;
