#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

std::vector<int> a, b;
std::vector<std::vector<int>> dp;

void testcase() {
  int n; std::cin >> n;
  a = std::vector<int>(n + 1, 0);
  b = std::vector<int>(n + 1, 0);
  dp = std::vector<std::vector<int>>(n + 1, std::vector<int>(n + 1, 0));

  for (int i = 1; i <= n; i++) {
    int ai; std::cin >> ai;
    a[i] = ai + a[i - 1];
  }
  
  for (int i = 1; i <= n; i++) {
    int bi; std::cin >> bi;
    b[i] = bi + b[i - 1];
  }
  
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      dp[i][j] = (a[i] - i) * (b[j] - j);

      if (i == 1 || j == 1)
        continue;
      
      for (int ka = 1; ka < i; ka++) {
        const int sa = a[i] - a[i - ka], sb = b[j] - b[j - 1];
        dp[i][j] = std::min(dp[i][j], dp[i - ka][j - 1] + (sa - ka) * (sb - 1));
      }
      
      for (int kb = 1; kb < j; kb++) {
        const int sa = a[i] - a[i - 1], sb = b[j] - b[j - kb];
        dp[i][j] = std::min(dp[i][j], dp[i - 1][j - kb] + (sa - 1) * (sb - kb));
      }
    }
  }

  std::cout << dp[n][n] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
