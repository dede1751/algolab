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

const int X = 0, Y = 1, Z = 2;

void p1(const int a, const int b) {
	Program lp(CGAL::SMALLER, true, 0, false, 0);

	lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
	lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
	lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

	lp.set_c(X, a); lp.set_c(Y, -b);

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible())
		std::cout << "no" << std::endl;
	else if (s.is_unbounded())
		std::cout << "unbounded" << std::endl;
	else 
		std::cout << (long)std::floor(-CGAL::to_double(s.objective_value())) << std::endl;
}

void p2(const int a, const int b) {
	Program lp(CGAL::LARGER, false, 0, true, 0);

	lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
	lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b);
	lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);

	lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible())
		std::cout << "no" << std::endl;
	else if (s.is_unbounded())
		std::cout << "unbounded" << std::endl;
	else 
		std::cout << (long)std::ceil(CGAL::to_double(s.objective_value())) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int p; std::cin >> p;
	while (p) {
		int a, b; std::cin >> a >> b;
		(p == 1) ? p1(a, b) : p2(a, b);
		std::cin >> p;
	}
}
