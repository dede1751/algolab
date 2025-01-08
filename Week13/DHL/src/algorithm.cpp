#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n;
std::vector<int> a, b;
std::vector<std::vector<int>> dp;

void testcase() {
	std::cin >> n;
	a = std::vector<int> (n);
	b = std::vector<int> (n);
	dp = std::vector<std::vector<int>> (n + 1, std::vector<int>(n + 1, 100*1000));

	for (int i = 0; i < n; i++)
		std::cin >> a[i];
	
	for (int i = 0; i < n; i++)
		std::cin >> b[i];
	
	dp[0][0] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			dp[i][j] = (a[i - 1] - 1) * (b[j - 1] - 1) + std::min(dp[i - 1][j], std::min(dp[i][j - 1], dp[i - 1][j - 1]));
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
