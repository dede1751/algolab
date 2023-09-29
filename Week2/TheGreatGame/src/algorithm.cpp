#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <array>

typedef std::vector<std::vector<int>> ADJ_LIST;
typedef std::vector<std::vector<std::array<int, 4>>> MEMO;

int search(ADJ_LIST const &adj, MEMO &mem, const int r, const int b, const int turn, const int color, const int n) {
	int turn_col = turn * 2 + color;

	if (r == n)
		return 0;
	else if (b == n)
		return 1;
	else if (mem[r][b][turn_col] != -1)
		return mem[r][b][turn_col];

	mem[r][b][turn_col] = !turn;
	for (const auto tgt: adj[color ? b : r])
		if (search(adj, mem, color ? r : tgt, color ? tgt : b, !turn, turn ? color : !color, n) == turn)
		{
			mem[r][b][turn_col] = turn;
			break;
		}

	return mem[r][b][turn_col];
}

void testcase() {
	int n, m, r, b; std::cin >> n >> m >> r >> b;
	ADJ_LIST adj(n + 1, std::vector<int>());
	MEMO mem(n + 1);
	for (auto &vec : mem) {
		vec.resize(n + 1);
		std::fill(vec.begin(), vec.end(), std::array<int, 4>({-1, -1, -1, -1}));
	}

	for (int i = 0; i < m; i++) {
		int u, v; std::cin >> u >> v; 
		adj[u].push_back(v);
	}
	std::cout << search(adj, mem, r, b, 0, 0, n) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
