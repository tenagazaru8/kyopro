/*
 * dat: データ配列
 * L: 区間クエリ左端 (半開区間)
 * R: 区間クエリ右端 (半開区間)
 * X: 区間クエリ判定値 (半開区間)
 * me: 区間情報の単位元
 * eval: 区間情報とdatを合成する関数 eval(区間値, データ1つ)
 * check: Xと区間情報からクエリの答えを計算 check(X, 左区間, 右区間)
 * re: クエリの答えの初期値
 */
template<typename Ta, typename Tx, typename Tm, typename F, typename Fc, typename Tr>
std::vector<Tr> OfflineDSTquery(std::vector<Ta> &dat, std::vector<int> L, std::vector<int> R, std::vector<Tx> X, Tm me, F eval, Fc check, Tr re)
{
  int n = dat.size(), qcnt = X.size();
  std::vector<int> qid(qcnt);
  for (int i = 0; i < qcnt; ++i) qid[i] = i;
  std::vector<Tr> ans(qcnt, re);
  std::vector<Tm> bs(n + 1, me);
  auto dfs = [&](auto f, int l, int r, int ql, int qr) -> void
  {
    if (l + 1 == r)
    {
      for (int i = ql; i < qr; ++i) ans[qid[i]] = check(X[i], me, eval(me, dat[l]));
      return;
    }
    int cnt = 0, mid = (l + r) / 2;
    for (int i = ql; i < qr; ++i)
    {
      if (R[i] <= mid)
      {
        std::swap(L[i], L[ql + cnt]);
        std::swap(R[i], R[ql + cnt]);
        std::swap(X[i], X[ql + cnt]);
        std::swap(qid[i], qid[ql + cnt]);
        ++cnt;
      }
    }
    int lid = ql + cnt;
    f(f, l, mid, ql, lid);
    cnt = 0;
    for (int i = lid; i < qr; ++i)
    {
      if (L[i] >= mid)
      {
        std::swap(L[i], L[lid + cnt]);
        std::swap(R[i], R[lid + cnt]);
        std::swap(X[i], X[lid + cnt]);
        std::swap(qid[i], qid[lid + cnt]);
        ++cnt;
      }
    }
    f(f, mid, r, lid, lid + cnt);
    bs[mid] = me;
    for (int i = mid - 1; i >= l; --i) bs[i] = eval(bs[i + 1], dat[i]);
    for (int i = mid; i < r; ++i) bs[i + 1] = eval(bs[i], dat[i]);
    for (int i = lid + cnt; i < qr; ++i) ans[qid[i]] = check(X[i], bs[L[i]], bs[R[i]]);
  };
  dfs(dfs, 0, n, 0, qcnt);
  return ans;
}