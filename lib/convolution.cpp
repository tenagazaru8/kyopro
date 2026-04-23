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
  static unsigned char lookup[1 << 23];
  static bool init = true;
  if (init)
  {
    invp2[0] = 1;
    for (int i = 1; i < bc; ++i) invp2[i] = invp2[i - 1] * 499122177 % mod;
    gp2[0] = 15311432;
    for (int i = 1; i < bc; ++i) gp2[i] = gp2[i - 1] * gp2[i - 1] % mod;
    invgp2[0] = 469870224;
    for (int i = 1; i < bc; ++i) invgp2[i] = invgp2[i - 1] * invgp2[i - 1] % mod;
    lookup[0] = -1, lookup[1] = 0;
    for (int i = 2; i < (1 << bc); ++i) lookup[i] = lookup[i >> 1] + 1;
    init = false;
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
  std::vector<int> br(n);
  for (int i = 0; i < n; ++i)
  {
    for (int tmp = i; tmp > 0;)
    {
      int lb = tmp & -tmp;
      br[i] += (1 << nb - lookup[lb] - 1);
      tmp -= lb;
    }
  }
  std::vector<long long> c(n);
  for(int i = 0; i < n; ++i) c[br[i]] = (long long)a[i] * b[i] % mod;
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
  for (int i = 0; i < n; ++i) if (br[i] < siz) res[br[i]] = c[i] * invp2[nb] % mod;
  return res;
}