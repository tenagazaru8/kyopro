template <typename T, typename X, typename Fe, typename Fa, typename Fm> class LazySegTree
{
private:
  const T et;
  const X ex;
  int num, nb;
  std::vector<T> dat;
  std::vector<X> lazy;
  Fe eval;
  Fa fa;
  Fm fm;
    
public:
  LazySegTree(std::vector<T> &v, T Et, X Ex, Fe valcal, Fa lazyupdate, Fm lazypropagation) : et(Et), ex(Ex), eval(valcal), fa(lazyupdate), fm(lazypropagation)
  {
    int siz = static_cast<int>(v.size());
    for (nb = 0; 1 << nb < siz; ++nb);
    num = 1 << nb;
    dat = std::vector<T> (num << 1, et);
    lazy = std::vector<X> (num, ex);
    for (int i = 0; i < siz; ++i) dat[i + num] = v[i];
    for (int i = num - 1; i > 0; --i) dat[i] = eval(dat[i << 1], dat[(i << 1) + 1]);
  }
  
  LazySegTree(int n, T Et, X Ex, Fe valcal, Fa lazyupdate, Fm lazypropagation) : et(Et), ex(Ex), eval(valcal), fa(lazyupdate), fm(lazypropagation)
  {
    for (nb = 0; 1 << nb < n; ++nb);
    num = 1 << nb;
    dat = std::vector<T> (num << 1, et);
    lazy = std::vector<X> (num, ex);
  }

  //update [left,right)
  void update(int left, int right, X val)
  {
    left = std::max(0, left), right = std::min(num, right);
    if (left >= right) return;
    left += num;
    right += num;
    for (int i = nb; i > 0; --i)
    {
      if (((left >> i) << i) != left) lazyupdate(left >> i);
      if (((right >> i) << i) != right) lazyupdate((right - 1) >> i);
    }
    for (int l = left, r = right; l < r; l >>= 1, r >>= 1)
    {
      if (l & 1) nodeapply(l++, val);
      if (r & 1) nodeapply(--r, val);
    }
    for (int i = 1; i <= nb; ++i)
    {
      if (((left >> i) << i) != left) dat[(left >> i)] = eval(dat[(left >> i) << 1], dat[((left >> i) << 1) + 1]);
      if (((right >> i) << i) != right) dat[(right - 1 >> i)] = eval(dat[(right - 1 >> i) << 1], dat[((right - 1 >> i) << 1) + 1]);
    }
  }
  void update(int id, X val)
  {
    id = std::max(0, id);
    id = std::min(num, id);
    update(id, id + 1, val);
  }

  //get value [left,right)
  T getval(int left, int right)
  {
    left = std::max(0, left), right = std::min(num, right);
    if (left >= right) return et;
    left += num;
    right += num;
    for (int i = nb; i > 0; --i)
    {
      if (((left >> i) << i) != left) lazyupdate(left >> i);
      if (((right >> i) << i) != right) lazyupdate(right - 1 >> i);
    }
    T ansl = et, ansr = et;
    for (int l = left, r = right; l < r; l >>= 1, r >>= 1)
    {
      if (l & 1) ansl = eval(ansl, dat[l++]);
      if (r & 1) ansr = eval(dat[--r], ansr);
    }
    return eval(ansl, ansr);
  }

  //get value [id]
  T getval(int id) {return getval(id, id + 1);}

  template <typename Fc>
  int maxright(int left, Fc check)
  {
    left = std::max(0, left);
    left = std::min(num, left);
    if (left == num) return num;
    int l = left + num;
    for (int i = nb; i > 0; --i) lazyupdate(l >> i);
    T now = et;
    while (true)
    {
      while ((l & 1) == 0) l >>= 1;
      T tmp = eval(now, dat[l]);
      if (!check(tmp))
      {
        while (l < num)
        {
          lazyupdate(l);
          l <<= 1;
          tmp = eval(now, dat[l]);
          if (check(tmp))
          {
            now = tmp;
            ++l;
          }
        }
        return l - num;
      }
      now = tmp;
      ++l;
      if ((l & -l) == l) return num;
    }
  }

  template <typename Fc>
  int minleft(int right, Fc check)
  {
    right = std::max(0, right);
    right = std::min(num, right);
    if (right == 0) return 0;
    int r = right + num;
    for (int i = nb; i > 0; --i) lazyupdate((r - 1) >> i);
    T now = et;
    while (true)
    {
      --r;
      while (r > 1 && (r & 1)) r >>= 1;
      T tmp = eval(dat[r], now);
      if (!check(tmp))
      {
        while (r < num)
        {
          lazyupdate(r);
          r = (r << 1) + 1;
          tmp = eval(dat[r], now);
          if (check(tmp))
          {
            now = tmp;
            --r;
          }
        }
        return r + 1 - num;
      }
      now = tmp;
      if ((r & -r) == r) return 0;
    }
  }

private:
  void nodeapply(int id, X val)
  {
    dat[id] = fa(dat[id], val);
    if (id < num) lazy[id] = fm(lazy[id], val);
  }
  void lazyupdate(int id)
  {
    nodeapply(id << 1, lazy[id]);
    nodeapply((id << 1) + 1, lazy[id]);
    lazy[id] = ex;
  }
};
template <class T, class X, class Fe, class Fa, class Fm>
LazySegTree(std::vector<T> &, T, X, Fe, Fa, Fm) -> LazySegTree<T, X, Fe, Fa, Fm>;
template <class T, class X, class Fe, class Fa, class Fm>
LazySegTree(int, T, X, Fe, Fa, Fm) -> LazySegTree<T, X, Fe, Fa, Fm>;
