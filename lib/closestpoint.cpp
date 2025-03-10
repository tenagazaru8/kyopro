#include <vector>
#include <algorithm>

std::pair<int, int> mindis(std::vector<std::pair<int, int>> xy)
{
    int siz = xy.size();
    std::vector<std::pair<std::pair<long long, long long>, int>> p(siz);
    for (int i = 0; i < siz; ++i) p[i] = {xy[i], i};
    std::sort(p.begin(), p.end());
    int id1 = -1, id2 = -1;
    long long ans = 9e18;
    auto cal = [&](auto f, int l, int r) -> long long
    {
        if (r - l <= 1) return ans;
        int mid = (l + r) / 2;
        long long x = p[mid].first.first;
        ans = std::min(std::min(f(f, l, mid), f(f, mid, r)), ans);
        std::inplace_merge(p.begin() + l, p.begin() + mid, p.begin() + r, [](const std::pair<std::pair<long long, long long>, int> &a, const std::pair<std::pair<long long, long long>, int> &b) -> bool {return a.first.second < b.first.second;});

        std::vector<int> b;
        for (int i = l; i < r; ++i)
        {
            if (abs(p[i].first.first - x) >= ans) continue;
            for (int j = 0; j < b.size(); ++j)
            {
                long long dx = p[i].first.first - p[b[b.size() - j - 1]].first.first;
                long long dy = p[i].first.second - p[b[b.size() - j - 1]].first.second;
                if (dy >= ans) break;
                long long tmp = dx * dx + dy * dy;
                if (ans > tmp)
                {
                    ans = tmp;
                    id1 = p[i].second;
                    id2 = p[b[b.size() - j - 1]].second;
                }
            }
            b.push_back(i);
        }
        return ans;
    };
    cal(cal, 0, siz);
    return std::make_pair(id1, id2);
}