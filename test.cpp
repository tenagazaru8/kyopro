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

int main()
{
    const int mod = 998244353;
    int n, m;
    cin >> n >> m;
    cout << pow(n, m, mod) << endl;
}