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
	long xs, ys, n; std::cin >> xs >> ys >> n;

  	Program lp (CGAL::SMALLER, false, 0, false, 0);
	const int T = 0, X = 1, Y = 2;
	lp.set_l(T, true, 0);
	lp.set_c(T, -1);

	for (int i = 0; i < n; i++) {
		long a, b, c, v; std::cin >> a >> b >> c >> v;
		long sign = a * xs + b * ys + c >= 0 ? 1 : -1;
		long norm = std::floor(std::sqrt(a * a + b * b));

		lp.set_a(X, i, -a * sign);
		lp.set_a(Y, i, -b * sign);
		lp.set_a(T, i, norm * v);
		lp.set_b(i, c * sign);
	}

  	Solution s = CGAL::solve_linear_program(lp, ET());
	std::cout << (long)std::floor(CGAL::to_double(*s.variable_values_begin())) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
