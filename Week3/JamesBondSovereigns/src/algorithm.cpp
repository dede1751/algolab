#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>

int memo[2001][2001];
int v[2001];

int search(const int i, const int j, const int p, const int m, const int k)
{
    if (i == j)
        return p == k ? v[i] : 0;
    else if (memo[i][j] != -1)
        return memo[i][j];

    int next = (p + 1) % m;
    int rest_first = search(i + 1, j, next, m, k), rest_end = search(i, j - 1, next, m, k);

    if (p == k)
        return memo[i][j] = std::max(v[i] + rest_first, v[j] + rest_end);
    else
        return memo[i][j] = std::min(rest_first, rest_end);
}


void testcase()
{
    int n, m, k; std::cin >> n >> m >> k;
    for (int i = 0; i <= n; i++)
        memset(&memo[i], -1, n * sizeof(int));

    for (int i = 0; i < n; ++i)
        std::cin >> v[i];
    
    std::cout << search(0, n - 1, 0, m, k) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i)
        testcase();
}
