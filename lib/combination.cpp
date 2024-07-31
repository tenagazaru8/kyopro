#include <iostream>
#include <vector>

struct Comb
{
  std::vector<long long> fac, finv, inv;
  const int mod;
  Comb(const int max, const int m) : mod(m), fac(max + 5), finv(max + 5), inv(max + 5)
  {
    fac[0] = fac[1] = finv[0] = finv[1] = inv[1] = 1;
    for (int i = 2; i < max + 5; ++i)
    {
      fac[i] = fac[i - 1] * i % mod;
      inv[i] = mod - inv[mod % i] * (mod / i) % mod;
      finv[i] = finv[i - 1] * inv[i] % mod;
    }
  }
  long long c(int n, int k)
  {
    if (n < k || n < 0 || k < 0) return 0;
    return fac[n] * (finv[k] * finv[n - k] % mod) % mod;
  }
  long long p(int n, int k)
  {
    if (n < k || n < 0 || k < 0) return 0;
    return fac[n] * finv[n - k] % mod;
  }
};

class Comb
{
  int siz, mod;
  std::vector<long long> _fac, _inv, _finv;
public:
  // 引数は (mod)
  Comb(int m) : mod(m), siz(2)
  {
    _fac.resize(siz);
    _inv.resize(siz);
    _finv.resize(siz);
    _fac[0] = _fac[1] = _inv[1] = _finv[0] = _finv[1] = 1;
  }
  long long p(int n, int k)
  {
    if (n < k || k < 0) return 0;
    resize(n + 1);
    return _fac[n] * _finv[n - k] % mod;
  }
  long long c(int n, int k)
  {
    if (n < k || k < 0) return 0;
    resize(n + 1);
    return _fac[n] * _finv[k] % mod * _finv[n - k] % mod;
  }
  long long inv(int n)
  {
    resize(n + 1);
    if (n < 0) return 0;
    else return _inv[n];
  }
  long long fac(int n)
  {
    resize(n + 1);
    if (n < 0) return 0;
    else return _fac[n];
  }
  long long finv(int n)
  {
    resize(n + 1);
    if (n < 0) return 0;
    else return _finv[n];
  }
private:
  void resize(int n)
  {
    if (n <= siz) return;
    for (int i = siz; i < n; ++i)
    {
      _fac.push_back((long long)i * _fac[i - 1] % mod);
      _inv.push_back(mod - _inv[mod % i] * (mod / i) % mod);
      _finv.push_back(_finv[i - 1] * _inv[i] % mod);
    }
    siz = n;
  }
};

struct Comb_mod
{
  std::vector<std::vector<int>> dp;
  int mod;
  Comb_mod(int p) : mod(p), dp(p, std::vector<int> (p))
  {
    dp[0][0] = 1;
    for (int i = 0; i < p - 1; ++i)
    {
      for (int j = 0; j <= i; ++j)
      {
        dp[i + 1][j] = (dp[i + 1][j] + dp[i][j]) % mod;
        dp[i + 1][j + 1] = (dp[i + 1][j + 1] + dp[i][j]) % mod;
      }
    }
  }
  int modc(long long n, long long k)
  {
    int ret = 1;
    while (n > 0)
    {
      ret = ret * dp[n % mod][k % mod] % mod;
      n /= mod;
      k /= mod;
    }
    return ret;
  }
};