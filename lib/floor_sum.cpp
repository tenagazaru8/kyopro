/*
以下の数式の結果をO(logN+logM)
i=0,1,...,n-1  Σfloor((a*i+b)/m)
*/
long long floor_sum(long long n, long long m, long long a, long long b)
{
  long long ans = 0;
  if (a < 0)
  {
    long long mi = a * (n - 1) + b;
    long long d = (mi % m + m) % m;
    mi -= d;
    return floor_sum(n, m, -a, d) + mi / m * n;
  }
  else if (a >= m)
  {
    ans += (n - 1) * n * (a / m) / 2;
    a %= m;
  }
  if (b < 0)
  {
    long long mi = b;
    long long d = (mi % m + m) % m;
    mi -= d;
    return floor_sum(n, m, a, d) + mi / m * n;
  }
  else if (b >= m)
  {
    ans += n * (b / m);
    b %= m;
  }
  long long ymax = (a * n + b) / m, xmax = (ymax * m - b);
  if (ymax == 0) return ans;
  ans += (n - (xmax + a - 1) / a) * ymax;
  ans += floor_sum(ymax, a, m, (a - xmax % a) % a);
  return ans;
}