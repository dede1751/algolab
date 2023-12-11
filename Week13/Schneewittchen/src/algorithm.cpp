#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n, m;
std::vector<std::vector<int>> adj, contents;
std::vector<int> dangerous, danger_ids;

void dfs(const int root, const int curr, Program& lp) {
	int idx = danger_ids[curr], root_idx = danger_ids[root];
	
	if (curr != root && idx >= 0)
		for (int j = 0; j < m; j++)
			lp.set_a(idx*m + j, root_idx*(m + 1) + j, -1);
	else {
		if (curr != root)
			for (int j = 0; j < m; j++)
				contents[root][j] += contents[curr][j];

		for (const int child: adj[curr])
			dfs(root, child, lp);
	}
	
	if (curr == root) {
		for (int j = 0; j < m; j++) {
			lp.set_a(root_idx*m + j, root_idx*(m + 1) + j, 2);
			lp.set_b(root_idx*(m + 1) + j, 2 * contents[root][j]);
		}
	}
}

void testcase() {
	std::cin >> n >> m;
	Program lp (CGAL::SMALLER, true, 0, false, 0);
	int idx = 1;
	
	adj = std::vector<std::vector<int>>(n);
	contents = std::vector<std::vector<int>>(n, std::vector<int>(m));
	dangerous = std::vector<int>();
	danger_ids = std::vector<int>(n, -1);
	
	for (int i = 0; i < n; i++) {
		int d; std::cin >> d;
		
		if (d >= 0) {
			danger_ids[i] = idx;
			dangerous.push_back(i);
			
			for (int j = 0; j < m; j++)
				lp.set_a(idx*m + j, idx*(m + 1) + m, 1);
			
			lp.set_b(idx*(m + 1) + m, d);
			idx++;
		}
		
		for (int j = 0; j < m; j++)
			std::cin >> contents[i][j];
	}
	
	for (int i = 0; i < n - 1; i++) {
		int u, v; std::cin >> u >> v;
		adj[v].push_back(u);
	}
	
	danger_ids[0] = 0;
	dangerous.push_back(0);
	for (const int root: dangerous)
		dfs(root, root, lp);
	
	for (int j = 0; j < m; j++) {
		int c, s, p; std::cin >> c >> s >> p;
		lp.set_a(j, idx*(m + 1) + j, -1);
		lp.set_a(idx*m + j, idx*(m + 1) + j, -1);
		lp.set_b(idx*(m + 1) + j, -c);

		lp.set_a(idx*m + j, idx*(m + 1) + m + j, 1);
		lp.set_b(idx*(m + 1) + m + j, s);

		lp.set_c(idx*m + j, p);
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible())
		std::cout << "Impossible!" << std::endl;
	else
		std::cout << (long)std::floor(CGAL::to_double(s.objective_value())) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}