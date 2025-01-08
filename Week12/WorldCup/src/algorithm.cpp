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

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Delaunay;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Circle_2 C;

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	int n, m, c; std::cin >> n >> m >> c;
  	Program lp (CGAL::SMALLER, true, 0, false, 0);

	std::vector<int> alcohol(n);
	std::vector<P> warehouses(n), stadiums(m);
	std::vector<std::vector<int>> revenues(n, std::vector<int>(m));

	for (int i = 0; i < n; i++) {
		int x, y, s, a; std::cin >> x >> y >> s >> a;
		warehouses[i] = P(x, y);

		lp.set_b(i, s);
		for (int j = 0; j < m; j++)
			lp.set_a(i*m + j, i, 1);

		alcohol[i] = a;
	}

	for (int j = 0; j < m; j++) {
		int x, y, d, u; std::cin >> x >> y >> d >> u;
		stadiums[j] = P(x, y);

		lp.set_b(n + j, d);
		lp.set_b(n + m + j, -d);
		lp.set_b(n + 2*m + j, u * 100);

		for (int i = 0; i < n; i++) {
			lp.set_a(i*m + j, n + j, 1);
			lp.set_a(i*m + j, n + m + j, -1);
			lp.set_a(i*m + j, n + 2*m + j, alcohol[i]);
		}
	}

	Delaunay t;
	t.insert(warehouses.begin(), warehouses.end());
	t.insert(stadiums.begin(), stadiums.end());

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int r; std::cin >> r;
			revenues[i][j] = 100 * r;
		}
	}

	for (int i = 0; i < c; i++) {
		long x, y, r; std::cin >> x >> y >> r;
		P center = P(x, y);
		C contour = C(center, r * r);

		if (!contour.has_on_bounded_side(t.nearest_vertex(center)->point()))
			continue;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				P w = warehouses[i], s = stadiums[j];

				if (contour.has_on_bounded_side(w) != contour.has_on_bounded_side(s))
					revenues[i][j] -= 1;
			}
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			lp.set_c(i*m + j, -revenues[i][j]);

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible())
		std::cout << "RIOT!" << std::endl;
	else
		std::cout << (long)std::floor(CGAL::to_double(-s.objective_value() / 100)) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
