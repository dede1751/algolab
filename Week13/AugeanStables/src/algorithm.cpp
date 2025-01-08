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

int n;
std::vector<long> f, c, k, l, m;

bool solves(const int a, const int p) {
	Program lp (CGAL::LARGER, true, 0, true, 1); 
	const int H1 = 0, H2 = 1, H3 = 2;

	lp.set_a(H1, n, -1); lp.set_b(n, -1);
	lp.set_a(H2, n + 1, -1); lp.set_b(n + 1, -1);
	lp.set_a(H3, n + 2, -1); lp.set_b(n + 2, -1);

	for (int i = 0; i < n; i++) {
		lp.set_a(H1, i, k[i] + a * a);
		lp.set_a(H2, i, l[i] + p * p);
		lp.set_a(H3, i, m[i] + a * p);
		lp.set_b(i, f[i] - c[i]);
	}

	return !CGAL::solve_nonnegative_linear_program(lp, ET()).is_infeasible();
}

void testcase() {
	std::cin >> n;
	long a[25] = {0}, p[25] = {0};
	f = std::vector<long>(n);
	c = std::vector<long>(n);
	k = std::vector<long>(n);
	l = std::vector<long>(n);
	m = std::vector<long>(n);

	for (int i = 0; i < n; i++) {
		std::cin >> f[i] >> c[i] >> k[i] >> l[i] >> m[i];
	}

	for (int i = 1; i <= 24; i++) {
		int x; std::cin >> x;
		a[i] = x + a[i - 1];
	}

	for (int i = 1; i <= 24; i++) {
		int x; std::cin >> x;
		p[i] = x + p[i - 1];
	}

	int best = 49;
	for (int at = 0; at < 25 && at <= best; at++) {
		int start = 0, end = std::min(25, best - at);
		while (start < end) {
			int pt = (start + end) / 2;

			if (solves(a[at], p[pt])) {
				best = at + pt;
				end = pt;
			} else
				start = pt + 1;
		}
	}
	
	if (best == 49)
		std::cout << "Impossible!" << std::endl;
	else
		std::cout << best << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
