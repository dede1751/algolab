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

using IT = int;
using ET = CGAL::Gmpz;
using Program = CGAL::Quadratic_program<IT>;
using Solution = CGAL::Quadratic_program_solution<ET>;

void testcase() {
	int n, m, h, w; std::cin >> n >> m >> h >> w;
	double ar = (double)h / w;

	Program lp(CGAL::SMALLER, true, 1, false, 0);
	std::vector<std::pair<int, int>> free(n);
	std::vector<std::pair<int, int>> old(m);

	for (int i = 0; i < n; i++) {
		int x, y; std::cin >> x >> y;
		free[i] = std::make_pair(x, y);
	}

	for (int i = 0; i < m; i++) {
		int x, y; std::cin >> x >> y;
		old[i] = std::make_pair(x, y);
	}

	for (int i = 0; i < n; i++) {
		int xa = free[i].first, ya = free[i].second;

		for (int j = i + 1; j < n; j++) {
			int xb = free[j].first, yb = free[j].second;
			int dx = std::abs(xb - xa), dy = std::abs(yb - ya);
			int idx = i*n + j;
			
			if (dx == 0 || (dy != 0 && ((double)dy / dx) >= ar)) {
				lp.set_a(i, idx, h);
				lp.set_a(j, idx, h);
				lp.set_b(idx, 2 * dy);
			} else {
				lp.set_a(i, idx, w);
				lp.set_a(j, idx, w);
				lp.set_b(idx, 2 * dx);
			}
		}

		double best = std::numeric_limits<double>::max();
		for (int j = 0; j < m; j++) {
			int xb = old[j].first, yb = old[j].second;
			int dx = std::abs(xb - xa), dy = std::abs(yb - ya);
			
			if (dx == 0 || (dy != 0 && ((double)dy / dx) >= ar)) {
				if (2 *(double)dy / h < best) {
					best = 2 * (double)dy / h;
					lp.set_a(i, n*n + i, h);
					lp.set_b(n*n + i, 2*dy - h);
				}
			} else {
				if (2 * (double)dx / w < best) {
					best = 2 * (double)dx / w;
					lp.set_a(i, n*n + i, w);
					lp.set_b(n*n + i, 2*dx - w);
				}
			}
		}

		lp.set_c(i, -2 * (h + w));
	}

	Solution s = CGAL::solve_linear_program(lp, ET());	
	std::cout << (long)std::ceil(-CGAL::to_double(s.objective_value())) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
