#include <iostream>
#include <vector>

template <typename T> class SegTree
{
private:
  const T e;
  int num;
  std::vector<T> dat;
  T (*const eval)(T &, T &) {};
public:
  SegTree(std::vector<T> &v, T E, T (*func)(T &, T &)) : e(E), eval(func)
  {
    int siz = static_cast<int>(v.size());
    for (num = 1; num < siz; num <<= 1);
    dat = std::vector<T> (2 * num - 1, e);
    for (int i = 0; i < siz; ++i) dat[i + num - 1] = v[i];
    for (int i = num - 2; i >= 0; --i) dat[i] = eval(dat[i * 2 + 1], dat[i * 2 + 2]);
  }
  SegTree(int n, T E, T (*func)(T &, T &)) : e(E), eval(func)
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
  int maxright(int left, T v, bool (*const check)(T &, T &))
  {
    T now = e;
    int id = left + num - 1;
    while (id < 2 * num - 1)
    {
      T tmp = eval(now, dat[id]);
      if (check(v, tmp))
      {
        if (id & 1) ++id;
        else
        {
          int tmpid = id + 2;
          if ((tmpid & -id) == tmpid) return num;
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
};