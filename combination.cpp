#include <iostream>
#include <vector>

using namespace std;

struct Comb
{
    int mod, n;
    vector<long long> fac, inv, finv;
    Comb(int siz, int m) : mod(m), n(siz), fac(siz + 5), inv(siz + 5), finv(siz + 5)
    {
        fac[0] = fac[1] = inv[1] = finv[0] = finv[1] = 1;
        for (int i = 2; i < siz + 5; ++i)
        {
            fac[i] = fac[i - 1] * i % mod;
            inv[i] = mod - (mod / i) * inv[mod % i] % mod;
            finv[i] = finv[i - 1] * inv[i] % mod;
        }
    }
    long long p(int n, int k)
    {
        if (n < k || k < 0) return 0;
        return fac[n] * finv[n - k] % mod;
    }
    long long c(int n, int k)
    {
        if (n < k || k < 0) return 0;
        return fac[n] * finv[k] % mod * finv[n - k] % mod;
    }
};