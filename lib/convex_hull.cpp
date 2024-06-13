#include <iostream>
#include <vector>
#include <algorithm>


std::vector<std::pair<int, int>> conv(std::vector<int> &x, std::vector<int> &y)
{
  int n = x.size();
  std::vector<std::pair<int, int>> xy(n), p(2 * n);
  for (int i = 0; i < n; ++i) xy[i] = {x[i], y[i]};
  sort(xy.begin(), xy.end());
  int k = 0;
  for (int i = 0; i < n; ++i)
  {
    while (k > 1 && (long long)(p[k - 1].first - p[k - 2].first) * (xy[i].second - p[k - 1].second) - (long long)(p[k - 1].second - p[k - 2].second) * (xy[i].first - p[k - 1].first) <= 0) --k;
    p[k++] = xy[i];
  }
  for (int i = n - 2, t = k; i >= 0; --i)
  {
    while (k > t && (long long)(p[k - 1].first - p[k - 2].first) * (xy[i].second - p[k - 1].second) - (long long)(p[k - 1].second - p[k - 2].second) * (xy[i].first - p[k - 1].first) <= 0) --k;
    p[k++] = xy[i];
  }
  p.resize(k - 1);
  return p;
}
