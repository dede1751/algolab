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

void testcase(const int n, const int m) {
	Program lp(CGAL::SMALLER, true, 0, false, 0);

	for (int i = 0; i < n; i++) {
		int min, max; std::cin >> min >> max;
		lp.set_b(2*i, -min);
		lp.set_b(2*i + 1, max);
	}

	for (int i = 0; i < m; i++) {
		int p; std::cin >> p;
		lp.set_c(i, p);

		for (int j = 0; j < n; j++) {
			int c; std::cin >> c;
			lp.set_a(i, 2*j, -c);
			lp.set_a(i, 2*j + 1, c);
		}
	}

	Solution s = CGAL::solve_linear_program(lp, ET());

	if (s.is_infeasible())
		std::cout << "No such diet." << std::endl;
	else
		std::cout << (long)std::floor(CGAL::to_double(s.objective_value())) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int n, m; std::cin >> n >> m;
	while (n) {
		testcase(n, m);
		std::cin >> n >> m;
	}
}
