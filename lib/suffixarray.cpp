//kitkatライブラリを普段の環境で動作するように修正したもの

#include <iostream>
#include <vector>
#include <algorithm>

template<typename S>
struct SuffixArray {
    // 接尾辞配列の構築
    // O(|S|log|S|)
    SuffixArray(const S& str) : s(str) {
        int n = s.size();
        sa.resize(n);
        for (int i = 0; i < n; ++i) sa[i] = i;
        sort(sa.begin(), sa.end(), [&](int a, int b) {
            if (s[a] == s[b]) return (a > b);
            return (s[a] < s[b]);
        });
        std::vector<int> classes(n), c(n), cnt(n);
        for (int i = 0; i < n; ++i) {
            c[i] = s[i];
        }
        for (int len = 1; len < n; len<<=1) {
            for (int i = 0; i < n; ++i) {
                if (i > 0 && c[sa[i-1]] == c[sa[i]] && sa[i-1] + len < n && c[sa[i-1]+len/2] == c[sa[i]+len/2]) {
                    classes[sa[i]] = classes[sa[i-1]];
                } else {
                    classes[sa[i]] = i;
                }
            }
            for (int i = 0; i < n; ++i) cnt[i] = i;
            for (int i = 0; i < sa.size(); ++i) c[i] = sa[i];
            for (int i = 0; i < n; ++i) {
                int s1 = c[i] - len;
                if (s1 >= 0) sa[cnt[classes[s1]]++] = s1;
            }
            swap(classes, c);
        }
    }
    // idx番目のSuffixArrayの値
    int operator[](int idx) const {
        return sa.at(idx);
    }
    // 文字列sが文字列tより辞書順で前に来るならtrue
    bool compare(const string& t, int si = 0, int ti = 0) {
        while (si < s.size() && ti < t.size()) {
            if (s[si] < t[ti]) return true;
            if (s[si] > t[ti]) return false;
            ++si, ++ti;
        }
        return (si >= s.size() && ti < t.size());
    }
    // 接尾辞配列上でT <= S[sa[i]:]となる最小のiを返す
    // O(|T|log|S|)
    int lower_bound(const string& t) {
        int low = -1, high = sa.size();
        while (high - low > 1) {
            int mid = (low + high) / 2;
            (compare(t, sa[mid]) ? low : high) = mid;
        }
        return high;
    }
    // 接尾辞配列上で T == S[sa[i]:] となるiの区間（半開区間）
    // O(|T|log|S|)
    pair<int,int> lower_upper_bound(string t) {
        int idx = lower_bound(t);
        t.push_back('~');
        int low = idx-1, high = sa.size();
        while (high - low > 1) {
            int mid = (low + high) / 2;
            (compare(t, sa[mid]) ? low : high) = mid;
        }
        return {idx, high};
    }
    std::vector<int> sa;
    S s;
};
template <class S>
SuffixArray(S) -> SuffixArray<S>;
template<typename S>
struct LCPArray {
    // LCP Arrayの構築
    // O(|S|)
    LCPArray(SuffixArray<S>& sa) {
        const S &s = sa.s;
        int n = s.size();
        rank.resize(n);
        for (int i = 0; i < n; ++i) {
            rank[sa[i]] = i;
        }
        lcp.resize(n);
        lcp[0] = 0;
        for (int i = 0, h = 0; i < n; ++i) {
            if (rank[i] + 1 < n) {
                for (int j = sa[rank[i]+1]; max(i,j)+h < n && s[i+h]==s[j+h]; ++h) {}
                lcp[rank[i]] = h;
                if (h > 0) --h;
            }
        }
    }
    // S[sa[idx-1]:]とS[sa[idx]:]のLCP長
    // O(1)
    int operator[](int idx) {
        return lcp.at(idx);
    }
    std::vector<int> lcp,rank;
};
template <class S>
LCPArray(SuffixArray<S>) -> LCPArray<S>;