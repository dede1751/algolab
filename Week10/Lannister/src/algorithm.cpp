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

typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	Program lp (CGAL::SMALLER, false, 0, false, 0); 
	const int B = 0, C = 1, D = 2, R = 3;

	long n, m, s; std::cin >> n >> m >> s;
	long x_sum = 0, y_sum = 0;
	std::vector<std::pair<long, long>> houses(n + m);

	for (int i = 0; i < n; i++) {
		long x, y; std::cin >> x >> y;
		lp.set_a(B, i, -y); lp.set_a(C, i, -1); lp.set_b(i, -x);
		houses[i] = {x, y};
		x_sum += x;
		y_sum += y;
	}

	for (int i = 0; i < m; i++) {
		long x, y; std::cin >> x >> y;
		lp.set_a(B, n + i, y); lp.set_a(C, n + i, 1); lp.set_b(n + i, x);
		houses[n + i] = {x, y};
		x_sum -= x;
		y_sum -= y;
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible()) {
		std::cout << "Yuck!" << std::endl;
		return;
	}

	if (s != -1) {
		lp.set_a(B, n + m, y_sum);
		lp.set_a(C, n + m, n - m);
		lp.set_b(n + m, s + x_sum);

		Solution sol = CGAL::solve_linear_program(lp, ET());
		if (sol.is_infeasible()) {
			std::cout << "Bankrupt!" << std::endl;
			return;
		}
	}

	int start = n + m + 1;
	for (int i = 0; i < n + m; i++) {
		long x = houses[i].first, y = houses[i].second;
		lp.set_a(B, start + 2*i, -x);
		lp.set_a(D, start + 2*i, 1);
		lp.set_a(R, start + 2*i, -1);
		lp.set_b(start + 2*i, y);

		lp.set_a(B, start + 2*i + 1, x);
		lp.set_a(D, start + 2*i + 1, -1);
		lp.set_a(R, start + 2*i + 1, -1);
		lp.set_b(start + 2*i + 1, -y);
	}

	lp.set_c(R, 1);
	sol = CGAL::solve_linear_program(lp, ET());
	std::cout << (long)std::ceil(CGAL::to_double(sol.objective_value())) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
