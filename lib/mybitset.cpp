#include <iostream>
#include <vector>

using namespace std;

class bitset
{
  const uint64_t all = -1;
  uint32_t num;
  int siz, modu;
  uint64_t *dat;
public:
  bitset(int n = 0, bool s = 0) : num(n)
  {
    siz = (num + 63) >> 6;
    modu = num & 63;
    dat = new uint64_t [siz];
    if (s)
    {
      for (int i = 0; i < siz - 1; ++i) dat[i] = all;
      dat[siz - 1] = (1ULL << modu) - 1;
    }
    else for (int i = 0; i < siz; ++i) dat[i] = 0;
  }
  bitset(const bitset &bs)
  {
    siz = bs.siz;
    num = bs.num;
    modu = bs.modu;
    dat = new uint64_t [siz];
    for (int i = 0; i < siz; ++i) dat[i] = bs.dat[i];
  }
  ~bitset() {delete [] dat;}
  
  // 0-index, bs[id]=flag
  void set(uint32_t id, uint64_t flag)
  {
    if (id >= num) exit(1);
    flag = !(!flag);
    int idx = id >> 6, shift = id & 63;
    dat[idx] &= (all ^ (!flag << shift));
    dat[idx] |= (flag << shift);
  }
  void set(uint32_t l, uint32_t r, uint64_t flag)
  {
    if (l > r || r > num) exit(1);
    flag = !(!flag);
    int idl = l >> 6, idr = r >> 6;
    if (idl == idr)
    {
      uint64_t mask1 = ((!flag << ((r & 63) - (l & 63))) - !flag) << (l & 63), mask2 = ((flag << ((r & 63) - (l & 63))) - flag) << (l & 63);
      dat[idl] &= all ^ mask1;
      dat[idl] |= mask2;
    }
    else
    {
      for (int i = idl + 1; i < idr; ++i)
      {
        dat[i] &= (0ULL - flag);
        dat[i] |= (0ULL - flag);
      }
      uint64_t mask1 = ((!flag << (64 - (l & 63))) - !flag) << (l & 63), mask2 = ((flag << (64 - (l & 63))) - flag) << (l & 63);
      dat[idl] &= all ^ mask1;
      dat[idl] |= mask2;
      mask1 = (!flag << (r & 63)) - !flag, mask2 = (flag << (r & 63)) - flag;
      dat[idr] &= all ^ mask1;
      dat[idr] |= mask2;
    }
  }
  
  bool get(uint32_t id)
  {
    if (id >= num) exit(1);
    return dat[id >> 6] & (1ULL << (id & 63));
  }
  // flip all
  void flip()
  {
    for (int i = 0; i < siz - 1; ++i) dat[i] ^= all;
    dat[siz - 1] = (dat[siz - 1] ^ all) & ((1ULL << modu) - 1);
  }
  // flip bs[id]
  void flip(uint32_t id)
  {
    if (id >= num) exit(1);
    dat[id >> 6] ^= (1ULL << (id & 63));
  }
  
  int count()
  {
    int ret = 0;
    for (int i = 0; i < siz; ++i) ret += __builtin_popcountll(dat[i]);
    return ret;
  }
  
  void resize(int n, bool s = 0)
  {
    num = n;
    siz = (num + 63) >> 6;
    modu = num & 63;
    delete [] dat;
    dat = new uint64_t [siz];
    for (int i = 0; i < siz; ++i) dat[i] = 0;
    if (s) flip();
  }
  
  bool isall()
  {
    for (int i = 0; i < siz - 1; ++i) if (dat[i] != all) return false;
    return dat[siz - 1] == (1ULL << modu) - 1;
  }
  bool isnone()
  {
    for (int i = 0; i < siz; ++i) if (dat[i] != 0) return false;
    return true;
  }
  int size() {return num;}
  void print()
  {
    printf("bitset<%d>: ", num);
    for (int i = 0; i < siz - 1; ++i) for (int j = 0; j < 64; ++j) printf("%d", (dat[i] & (1ULL << j)) != 0);
    for (int i = 0; i < modu; ++i) printf("%d", (dat[siz - 1] & (1ULL << i)) != 0);
    cout << endl;
  }
  
  
  inline bitset& operator=(const bitset &a)
  {
    num = a.num;
    modu = a.modu;
    siz = a.siz;
    delete[] dat;
    dat = new uint64_t [siz];
    for (int i = 0; i < siz; ++i) dat[i] = a.dat[i];
    return *this;
  }
  inline bitset& operator&=(const bitset &a)
  {
    if (a.num != num) exit(2);
    for (int i = 0; i < siz; ++i) dat[i] &= a.dat[i];
    return *this;
  }
  inline bitset& operator|=(const bitset &a)
  {
    if (a.num != num) exit(2);
    for (int i = 0; i < siz; ++i) dat[i] |= a.dat[i];
    return *this;
  }
  inline bitset& operator^=(const bitset &a)
  {
    if (a.num != num) exit(2);
    for (int i = 0; i < siz; ++i) dat[i] ^= a.dat[i];
    return *this;
  }
  inline bitset operator&(const bitset &a) {return bitset(*this) &= a;}
  inline bitset operator|(const bitset &a) {return bitset(*this) |= a;}
  inline bitset operator^(const bitset &a) {return bitset(*this) ^= a;}
  
  // inline bitset operator<<(const int s)
  // {
    
  // }

  // inline bitset operator<<=(const bitset &a, const bitset &b)
  // {
  //   // bitset bs
  //   // int siz = max(a.size(), b.size());
  // }
  // inline bitset& operator>>=(const int a)
  // {
  //   // if (a.num != num) exit(2);
  //   // for (int i = 0; i < siz; ++i) dat[i] ^= a.dat[i];
  //   // return *this;
  // }
  inline bitset operator~()
  {
    bitset ret = *this;
    ret.flip();
    return ret;
  }
  inline bool operator[] (const int id) {return dat[id >> 6] & (1ULL << (id & 63));}
};

int main()
{
  int n, m;
  cin >> n >> m;
  bitset bs1(n, 1), bs2(m);
  bs1.print();
  bs1 = bs2;
  bs1.print();
}