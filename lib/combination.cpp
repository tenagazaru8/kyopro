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

struct Comb
{
  int siz, mod;
  std::vector<long long> fac, inv, finv;
  Comb(int m) : mod(m), siz(2)
  {
    fac.resize(2);
    inv.resize(2);
    finv.resize(2);
    fac[0] = fac[1] = inv[1] = finv[0] = finv[1] = 1;
  }
  long long p(int n, int k)
  {
    if (n < k || k < 0) return 0;
    resize(n + 2);
    return fac[n] * finv[n - k] % mod;
  }
  long long c(int n, int k)
  {
    if (n < k || k < 0) return 0;
    resize(n + 2);
    return fac[n] * finv[k] % mod * finv[n - k] % mod;
  }
  long long h(int n, int k) {return c(n + k - 1, k);}
private:
  void resize(int n)
  {
    if (n <= siz) return;
    for (int i = siz; i < n; ++i)
    {
      fac.push_back((long long)i * fac[i - 1] % mod);
      inv.push_back(mod - inv[mod % i] * (mod / i) % mod);
      finv.push_back(finv[i - 1] * inv[i] % mod);
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