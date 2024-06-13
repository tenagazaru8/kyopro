#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace matrix
{
    std::vector<std::vector<long long>> mul(std::vector<std::vector<long long>> &v1, std::vector<std::vector<long long>> &v2, long long mod = 1001001001001001001)
    {
        std::vector<std::vector<long long>> ans;
        if (v1.size() != v2[0].size()) return ans;
        ans.resize(v1.size(), std::vector<long long> (v2[0].size()));
        for (int i = 0; i < v1.size(); ++i)
        {
            for (int j = 0; j < v2[0].size(); ++j)
            {
                long long tmp = 0;
                for (int k = 0; k < v2.size(); ++k)
                {
                    tmp += v1[i][k] * v2[k][j];
                    tmp %= mod;
                }
                ans[i][j] = tmp;
            }
        }
        return ans;
    }
    std::vector<std::vector<long long>> pow(std::vector<std::vector<long long>> &v, long long x, long long mod = 1001001001001001001)
    {
        std::vector<std::vector<long long>> ans(v.size(), std::vector<long long> (v.size()));
        for (int i = 0; i < v.size(); ++i) ans[i][i] = 1;
        for (; x > 0; x >>= 1)
        {
            if (x & 1LL) ans = mul(v, ans, mod);
            v = mul(v, v, mod);
        }
        return ans;
    }
};

using namespace std;

int main()
{
    int n;
    long long k;
    const int MOD = 998244353;
    cin >> n >> k;
    vector<vector<long long>> a(n, vector<long long> (n));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> a[i][j];
        }
    }
}