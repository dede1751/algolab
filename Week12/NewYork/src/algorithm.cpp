#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

typedef std::priority_queue<std::pair<int,int>> prio_q;

int n, m, k;
bool one_safe;

prio_q q_max, q_min;
std::vector<int> prev, temps, parent;
std::vector<bool> leaf, safe, visited;

void testcase() {
	std::cin >> n >> m >> k;
	one_safe = false;

	prev = std::vector<int>(n, -1);
	temps = std::vector<int>(n);
	parent = std::vector<int>(n);

	leaf = std::vector<bool>(n, true);
	safe = std::vector<bool>(n, false);
	visited = std::vector<bool>(n, false);

	for (int i = 0; i < n; i++)
		std::cin >> temps[i];

	for (int i = 0; i < n - 1; i++) {
		int u, v; std::cin >> u >> v;
		prev[v] = u;
		leaf[u] = false;
	}

	for (int i = 0; i < n; i++) {
		if (leaf[i]) {
			int curr = i;
			q_max = prio_q();
			q_min = prio_q();

			for (int j = 0; curr != -1; j++, curr = prev[curr]) {
				parent[j] = curr;
				q_max.push({temps[curr], j});
				q_min.push({-temps[curr], j});

				while(q_max.top().second + m <= j)
					q_max.pop();

				while(q_min.top().second + m <= j)
					q_min.pop();

				if (j + 1 >= m) {
					if (q_max.top().first + q_min.top().first <= k)
						one_safe = safe[curr] = true;
					
					if (visited[parent[j + 1 - m]])
						break;

					visited[parent[j + 1 - m]] = true;
				}
			}
		}
	}

	if (one_safe) {
		for (int i = 0; i < n; i++)
			if (safe[i])
				std::cout << i << " ";

		std::cout << std::endl;
	} else
		std::cout << "Abort mission" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
