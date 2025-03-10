template <typename T> class BIT
{
private:
  T e;
  int num;
  std::vector<T> dat;
  T (*eval)(T &, T &) {};
public:
  BIT() : e(0), num(0), dat(std::vector<int>(0)), eval([](int &a, int &b) -> int {return a + b;}){}
  BIT(const BIT<T> &bit) : e(bit.e), num(bit.num), dat(bit.dat), eval(bit.eval){}
  BIT(std::vector<T> &v, T E, T (*func)(T &, T &)) : e(E), eval(func)
  {
    num = static_cast<T>(v.size());
    dat = std::vector<T> (num + 1, e);
    for (int i = 0; i < num; ++i) dat[i + 1] = v[i];
    for (int i = 1; i <= num; ++i)
    {
      int j = i + (i & -i);
      if (j <= num) dat[j] = eval(dat[i], dat[j]);
    }
  }
  BIT(int n, T E, T (*func)(T &, T &)) : e(E), eval(func)
  {
    num = n;
    dat = std::vector<T> (num + 1, e);
  }
  //1-index
  void update(int i, T val) {for (; i <= num; i += i & -i) dat[i] = eval(dat[i], val);}
  //[0,right]
  T getval(int right)
  {
    T ans = e;
    for (; right > 0; right &= right - 1) ans = eval(ans, dat[right]);
    return ans;
  }
  int minright(T val)
  {
    if (val <= 0) return 0;
    int ret = 0, tmp = 1;
    for (int i = num; i > 0; i -= i & -i) tmp = i;
    for (; tmp > 0; tmp >>= 1)
    {
      if (ret + tmp <= num && dat[ret + tmp] < val)
      {
        ret += tmp;
        val -= dat[ret];
      }
    }
    return ret + 1;
  }
}; 