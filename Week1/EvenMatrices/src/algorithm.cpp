#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::vector<std::vector<int>> IMAT;

void testcase() {
	int n; std::cin >> n;
	IMAT m(n + 1, std::vector<int>(n + 1, 0));

	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++) {
			int a; std::cin >> a;
			m[i][j] = a - m[i - 1][j - 1] + m[i - 1][j] + m[i][j - 1];
		}

	int sum = 0;
	for (int i1 = 1; i1 <= n; i1++)
		for (int i2 = i1; i2 <= n; i2++){
			int even = 1, odd = 0;

			for (int j = 1, s = 0; j <= n; j++){
				s += m[i2][j] + m[i1 - 1][j - 1] - m[i1 - 1][j] - m[i2][j - 1];

				if (s % 2) {
					odd++;
				} else {
					even++;
				}
			}

			sum += (even * (even - 1)) / 2 + (odd * (odd - 1)) / 2;
		}

	std::cout << sum << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
