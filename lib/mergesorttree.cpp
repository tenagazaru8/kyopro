#include <iostream>
#include <vector>
#include <algorithm>

template<typename T> class mergeSortTree
{
private:
  int num;
  std::vector<std::vector<T>> dat;
public:
  mergeSortTree(std::vector<T> &v)
  {
    int siz = static_cast<int>(v.size());
    for (num = 1; num < siz; num <<= 1);
    dat.resize(2 * num - 1);
    for (int i = 0; i < siz; ++i) dat[i + num - 1].push_back(v[i]);
    for (int i = num - 2; i >= 0; --i) merge(dat[i * 2 + 1].begin(), dat[i * 2 + 1].end(), dat[i * 2 + 2].begin(), dat[i * 2 + 2].end(), std::back_inserter(dat[i]));
  }
  long long getval(int left, int right, T x) // [left : right)のx以下の個数
  {
    long long ans = 0;
    for (left += num - 1, right += num - 1; left < right; left >>= 1, right >>= 1)
    {
      if (!(left & 1)) ans += cal(left, x);
      if (--right & 1) ans += cal(right, x);
    }
    return ans;
  }

  long long cal(int id, T x)
  {
    return upper_bound(dat[id].begin(), dat[id].end(), x) - dat[id].begin();
  }
};