#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>

void testcase() {
  int n, m, k; std::cin >> n >> m >> k;
  std::vector<int> coins(n, 0);
  std::vector<std::vector<int>> dp(n , std::vector<int>(n, 0));
  
  for (int i = 0; i < n; i++)
    std::cin >> coins[i];
    
  if ((n - 1) % m == k)
    for (int i = 0; i < n; i++)
        dp[i][i] = coins[i];
  
  for (int i = n - 2; i >= 0; i--) {
    for (int j = i + 1; j < n; j++) {
      int curr_player = (n + i - j - 1) % m;
      
      if (curr_player == k) 
        dp[i][j] = std::max(coins[i] + dp[i + 1][j], coins[j] + dp[i][j - 1]);
      else
        dp[i][j] = std::min(dp[i + 1][j], dp[i][j - 1]);
    }
  }
  
  std::cout << dp[0][n - 1] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
