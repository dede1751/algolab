#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	int a, s, b, e; std::cin >> a >> s >> b >> e;
  	Program lp (CGAL::LARGER, true, 0, false, 0);
	std::vector<P> particles(a), shots(s), hunters(b);
	std::vector<int> densities(a);
	std::vector<K::FT> max_radius(s, std::numeric_limits<long>::max());

	for (int i = 0; i < a; i++) {
		int x, y; std::cin >> x >> y >> densities[i];
		particles[i] = P(x, y);
	}

	for (int i = 0; i < s; i++) {
		int x, y; std::cin >> x >> y;
		shots[i] = P(x, y);
	}

	for (int i = 0; i < b; i++) {
		int x, y; std::cin >> x >> y;
		hunters[i] = P(x, y);
	}

	Triangulation t;
  	t.insert(hunters.begin(), hunters.end());

	for (int i = 0; i < s && b != 0; i++) {
		P closest = t.nearest_vertex(shots[i])->point();
		max_radius[i] = CGAL::squared_distance(shots[i], closest);
	}

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < s; j++) {
			K::FT dist = CGAL::squared_distance(particles[i], shots[j]);
			if (dist > max_radius[j])
				continue;
			
			lp.set_a(j, i, IT(1, std::max(K::FT(1), dist)));
		}

		lp.set_b(i, IT(densities[i]));
	}

	for (int j = 0; j < s; j++) {
		lp.set_a(j, a, IT(-1));
	}
	lp.set_b(a, IT(-e));

	Solution sol = CGAL::solve_linear_program(lp, ET());
	std::cout << (sol.is_infeasible() ? "n" : "y") << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
