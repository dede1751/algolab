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

void testcase(const int n) {
	int d; std::cin >> d;
	Program lp(CGAL::SMALLER, false, 0, false, 0);

	const int R = 0;
	lp.set_l(R, true, 0);
	lp.set_c(R, -1);

	for (int i = 0; i < n; i++) {
		int norm = 0;

		for (int j = 1; j <= d; j++) {
			int a; std::cin >> a;
			lp.set_a(j, i, a);
			norm += a * a;
		}
		norm = std::floor(std::sqrt(norm));
		lp.set_a(R, i, norm);

		int b; std::cin >> b;
		lp.set_b(i, b);
	}
	
	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible())
		std::cout << "none" << std::endl;
	else if (s.is_unbounded())
		std::cout << "inf" << std::endl;
	else
		std::cout << (long)std::floor(CGAL::to_double(*s.variable_values_begin())) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int n; std::cin >> n;
	while (n) {
		testcase(n);
		std::cin >> n;
	}
}
