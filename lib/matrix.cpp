#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

std::vector<std::vector<long long>> mul(const std::vector<std::vector<long long>> &v1, const std::vector<std::vector<long long>> &v2, long long mod = 1001001001001001001)
{
    std::vector<std::vector<long long>> ans;
    if (v1.size() != v2[0].size()) return ans;
    ans.resize(v1.size(), std::vector<long long> (v2[0].size()));
    for (int i = 0; i < v1.size(); ++i) for (int j = 0; j < v2[0].size(); ++j) for (int k = 0; k < v2.size(); ++k) ans[i][j] = (ans[i][j] + v1[i][k] * v2[k][j]) % mod;
    return ans;
}

std::vector<std::vector<long long>> pow(std::vector<std::vector<long long>> v, long long k, long long mod = 1001001001001001001)
{
    std::vector<std::vector<long long>> ans;
    if (v.size() != v[0].size()) return ans;
    ans.resize(v.size(), std::vector<long long> (v.size()));
    for (int i = 0; i < v.size(); ++i) ans[i][i] = 1;
    for (; k > 0; k >>= 1, v = mul(v, v, mod)) if (k & 1) ans = mul(v, ans, mod);
    return ans;
}
