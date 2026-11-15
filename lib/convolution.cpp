template<typename T>
std::vector<T> convolution(std::vector<T> a, std::vector<T> b)
{
  if (a.empty() || b.empty()) return {};
  const int mod = 998244353, bc = 23;
  int siz = a.size() + b.size() - 1, n = 1, nb = 0;
  while (siz > n)
  {
    n <<= 1;
    ++nb;
  }
  a.resize(n);
  b.resize(n);
  static long long invgp2[23], gp2[23], invp2[23];
  static unsigned int br[1 << 23];
  static bool init = true;
  if (init)
  {
    init = false;
    invp2[0] = 1;
    for (int i = 1; i < bc; ++i) invp2[i] = invp2[i - 1] * 499122177 % mod;
    gp2[0] = 15311432;
    for (int i = 1; i < bc; ++i) gp2[i] = gp2[i - 1] * gp2[i - 1] % mod;
    invgp2[0] = 469870224;
    for (int i = 1; i < bc; ++i) invgp2[i] = invgp2[i - 1] * invgp2[i - 1] % mod;
    for (int i = 0; i < 1 << bc; ++i)
    {
      unsigned int ni = i;
      ni = ((ni >> 16) & 0x0000ffff) | ((ni & 0x0000ffff) << 16);
      ni = ((ni >> 8) & 0x00ff00ff) | ((ni & 0x00ff00ff) << 8);
      ni = ((ni >> 4) & 0x0f0f0f0f) | ((ni & 0x0f0f0f0f) << 4);
      ni = ((ni >> 2) & 0x33333333) | ((ni & 0x33333333) << 2);
      ni = ((ni >> 1) & 0x55555555) | ((ni & 0x55555555) << 1);
      br[i] = ni >> 9;
    }
  }
  for (int tb = nb - 1; tb >= 0; --tb)
  {
    int bt = (1 << tb);
    std::vector<long long> gpp(bt);
    gpp[0] = 1;
    for (int i = 1; i < bt; ++i) gpp[i] = gpp[i - 1] * gp2[bc - tb - 1] % mod;
    for (int i = 0; i < n; i += (bt << 1))
    {
      for (int j = i; j < i + bt; ++j)
      {
        long long w = gpp[j - i];
        long long s = a[j], t = a[j + bt];
        a[j] = s + t;
        if (a[j] >= mod) a[j] -= mod;
        a[j + bt] = (s - t + mod) * w % mod;
        s = b[j];
        t = b[j + bt];
        b[j] = s + t;
        if (b[j] >= mod) b[j] -= mod;
        b[j + bt] = (s - t + mod) * w % mod;
      }
    }
  }
  std::vector<long long> c(n);
  for(int i = 0; i < n; ++i) c[br[i] >> bc - nb] = (long long)a[i] * b[i] % mod;
  for (int tb = nb - 1; tb >= 0; --tb)
  {
    int bt = (1 << tb);
    std::vector<long long> gpp(bt);
    gpp[0] = 1;
    for (int i = 1; i < bt; ++i) gpp[i] = gpp[i - 1] * invgp2[bc - tb - 1] % mod;
    for (int i = 0; i < n; i += (bt << 1))
    {
      for (int j = i; j < i + bt; ++j)
      {
        long long w = gpp[j - i];
        long long s = c[j], t = c[j + bt];
        c[j] = s + t;
        if (c[j] >= mod) c[j] -= mod;
        c[j + bt] = (s - t + mod) * w % mod;
      }
    }
  }
  std::vector<T> res(siz);
  for (int i = 0; i < n; ++i) if ((br[i] >> bc - nb) < siz) res[br[i] >> bc - nb] = c[i] * invp2[nb] % mod;
  return res;
}

template<typename T>
std::vector<T> inversion(const std::vector<T> &v)
{
  const int mod = 998244353;
  int n = v.size();
  std::vector<T> ret(1), f;
  ret[0] = 1;
  f.reserve(n);
  for (long long a = v[0] % mod, k = mod - 2; k > 0; k >>= 1, a = a * a % mod) if (k & 1) ret[0] = ret[0] * a % mod;
  for (int w = 1; w < n; w <<= 1)
  {
    int fn = std::min(2 * w, n);
    while (f.size() < fn) f.push_back(v[f.size()]);
    auto res = convolution(f, ret);
    res.resize(fn);
    res[0] = (mod + 2 - res[0]) % mod;
    for (int i = 1; i < res.size(); ++i) res[i] = (mod - res[i]) % mod;
    ret = convolution(ret, res);
    ret.resize(fn);
  }
  return ret;
}

template<typename T>
std::vector<T> div(const std::vector<T> &a, const std::vector<T> &b)
{
  const int mod = 998244353;
  std::vector<T> aa = a, bb = b;
  while (!aa.empty() && aa.back() == 0) aa.pop_back();
  while (!bb.empty() && bb.back() == 0) bb.pop_back();
  if (aa.empty() || aa.size() < bb.size()) return {0};
  if (bb.empty())
  {
    cout << "多項式の除算で0除算" << endl;
    exit(1);
  }
  int k = aa.size() - bb.size() + 1;
  std::reverse(aa.begin(), aa.end());
  std::reverse(bb.begin(), bb.end());
  aa.resize(k);
  bb.resize(k, 0);
  auto ib = inversion(bb);
  auto q = convolution(aa, ib);
  q.resize(k);
  std::reverse(q.begin(), q.end());
  while (!q.empty() && q.back() == 0) q.pop_back();
  return q;
}

template<typename T>
std::vector<T> poly_mod(const std::vector<T> &a, const std::vector<T> &b)
{
  const int mod = 998244353;
  std::vector<T> aa = a, bb = b;
  while (!aa.empty() && aa.back() == 0) aa.pop_back();
  while (!bb.empty() && bb.back() == 0) bb.pop_back();
  if (aa.empty()) return {0};
  if (aa.size() < bb.size()) return aa;
  if (bb.empty())
  {
    cout << "多項式の除算で0除算" << endl;
    exit(1);
  }
  auto q = div(aa, bb);
  auto qb = convolution(q, bb);
  aa.resize(std::max(aa.size(), qb.size()), 0);
  for (int i = 0; i < qb.size(); ++i) aa[i] = (aa[i] - qb[i] + mod) % mod;
  while (aa.size() > 1 && aa.back() == 0) aa.pop_back();
  return aa;
}




template<typename T>
T fiduccia(const std::vector<T> &a, const std::vector<T> &c, long long k)
{
  const int mod = 1000000007;
  std::vector<T> g(c.size() + 1);
  g[c.size()] = 1;
  for (int i = 0; i < c.size(); ++i) g[i] = -c[i] % mod + mod;
  auto powmod = [&](auto f, long long r) -> std::vector<T>
  {
    std::vector<T> ret(c.size());
    if (r < c.size())
    {
      ret[r] = 1;
      return ret;
    }
    ret = f(f, r / 2);
    ret = poly_mod(convolution(ret, ret), g);
    ret.resize(c.size());
    if (r & 1)
    {
      long long tmp = ret[c.size() - 1];
      for (int i = ret.size() - 1; i > 0; --i) ret[i] = ret[i - 1];
      ret[0] = 0;
      if (tmp != 0)
      {
        for (int i = 0; i < c.size(); ++i) ret[i] = (ret[i] + tmp * c[i]) % mod;
      }
    }
    return ret;
  };
  auto res = powmod(powmod, k);
  T ans = 0;
  for (int i = 0; i < c.size(); ++i) ans = (ans + res[i] * a[i]) % mod;
  return ans;
}
