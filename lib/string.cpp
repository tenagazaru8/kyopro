#include <iostream>
#include <vector>

using namespace std;

template<typename S>
vector<int> Z_algorithm(const S &s)
{
  int c = 0, n = s.size();
  vector<int> ret(n);
  for (int i = 1; i < n; ++i)
  {
    int l = i - c;
    if (i + ret[l] < c + ret[c]) ret[i] = ret[l];
    else
    {
      int j = max(0, c + ret[c] - i);
      while (i + j < n && s[i + j] == s[j]) ++j;
      ret[i] = j;
      c = i;
    }
  }
  ret[0] = n;
  return ret;
}

template<typename S>
vector<int> kai(const S s)
{
  int n = s.size(), c = 0;
  vector<int> ret(n);
  for (int i = 0; i < n; ++i)
  {
    int l = 2 * c - i;
    if (l >= 0 && c + ret[c] > i + ret[l]) ret[i] = ret[l];
    else
    {
      int j = c + ret[c] - i;
      while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) ++j;
      ret[i] = j;
      c = i;
    }
  }
  return ret;
}
