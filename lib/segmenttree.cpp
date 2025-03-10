#include <iostream>
#include <vector>
#include <queue>
#include <stack>

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