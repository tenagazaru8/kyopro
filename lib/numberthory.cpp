#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long pow(long long a, long long k, long long m)
{
  long long ret = 1;
  for (a %= m; k > 0; k >>= 1, a = a * a % m) if (k & 1) ret = ret * a % m;
  return ret;
}

long long extgcd(long long a, long long b, long long &x, long long &y)
{
  long long x1 = 0, y1 = 1;
  x = 1, y = 0;
  while (b != 0)
  {
    long long n = a / b;
    a %= b; swap(a, b);
    x -= x1 * n; swap(x, x1);
    y -= y1 * n; swap(y, y1);
  }
  return a;
}

pair<long long, long long> crt(long long b1, long long m1, long long b2, long long m2)
{
  long long x = 1, y = 0, x1 = 0, y1 = 1, g = m1, b = m2;
  while (b != 0)
  {
    long long n = g / b;
    g %= b; swap(g, b);
    x -= x1 * n; swap(x, x1);
    y -= y1 * n; swap(y, y1);
  }
  if ((b2 - b1) % g != 0) return {0, -1};
  long long lc = m1 / g * m2;
  return {((b1 + m1 * ((b2 - b1) / g * x % (m2 / g))) % lc + lc) % lc, lc};
}

bool isprime(long long n)
{
    if (n == 2) return true;
    else if (n == 1 || n % 2 == 0) return false;
    auto pow = [&](__int128_t a, long long k) -> long long
    {
        __int128_t ret = 1;
        for (a %= n; k > 0; k >>= 1, a = a * a % n) if (k & 1) ret = ret * a % n;
        return ret;
    };
    long long m = n - 1;
    int s = 0;
    while (!((m >> s) & 1)) ++s;
    long long d = (m >> s);
    for (long long p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
    {
        if (p == n) return true;
        long long x = pow(p, d);
        int r = 0;
        if (x == 1) continue;
        while (x != m)
        {
            x = pow(x, 2);
            ++r;
            if (x == 1 || r == s) return false;
        }
    }
    return true;
}

long long lagrange(int siz, int mod)
{
  vector<long long> y(siz, 1);
  for (int i = 0; i < siz - 1; ++i) y[i + 1] = (y[i] + pow(i + 2, n, mod)) % mod;
  for (int i = 0; i < siz; ++i) cout << y[i] << " \n"[i == siz - 1];
  auto lagrange = [&](long long t) -> long long
  {
    if (t < siz) return y[t];
    long long ret = 0;
    vector<long long> dp(siz, 1), pd(siz, 1);
    for (int i = 0; i < siz - 1; ++i) dp[i + 1] = (t - i) % mod * dp[i] % mod;
    for (int i = siz - 1; i > 0; --i) pd[i - 1] = (t - i) % mod * pd[i] % mod;
    for (int i = 0; i < siz; ++i)
    {
      long long tmp = y[i] * dp[i] % mod * pd[i] % mod * cb.finv[i] % mod * cb.finv[siz - 1 - i] % mod;
      ret += ((siz - 1 - i) & 1) ? -tmp : tmp; 
      ret = (ret + mod) % mod;
    }
    return ret;
  };
  cout << lagrange(m - 1) << endl;
}

int main()
{
  
}