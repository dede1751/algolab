#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using P = K::Point_2;
using S = K::Segment_2;
using L = K::Line_2;
using T = K::Triangle_2;

void testcase() {
	int m, n; std::cin >> m >> n;
	std::vector<S> legs(m - 1);
	std::vector<std::vector<P>> fragments(n, std::vector<P>(6));
	std::vector<std::vector<int>> coverages(n + 1, std::vector<int>(m - 1, 0));

	int x, y; std::cin >> x >> y;
	P prev(x, y);
	for (int i = 0; i < m - 1; i++) {
		int x, y; std::cin >> x >> y;
		P next(x, y);

		legs[i] = S(prev, next);
		prev = next;
	}

	for (int i = 0; i < n; i++) {
		int x11, y11, x12, y12; std::cin >> x11 >> y11 >> x12 >> y12; 
		int x21, y21, x22, y22; std::cin >> x21 >> y21 >> x22 >> y22; 
		int x31, y31, x32, y32; std::cin >> x31 >> y31 >> x32 >> y32;
		P p1(x11, y11), p2(x12, y12), p3(x21, y21), p4(x22, y22), p5(x31, y31), p6(x32, y32);

		if (CGAL::right_turn(p1, p2, p3))
			std::swap(p1, p2);
		
		if (CGAL::right_turn(p3, p4, p5))
			std::swap(p3, p4);
		
		if (CGAL::right_turn(p5, p6, p1))
			std::swap(p5, p6);

		fragments[i] = std::vector<P>({p1, p2, p3, p4, p5, p6});
	}

	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < m - 1; j++) {
			coverages[i][j] = coverages[i - 1][j] + 1;
			
			for (int side = 0; side < 3; side++) {
				P p1 = fragments[i - 1][side * 2], p2 = fragments[i - 1][side * 2 + 1];

				if (CGAL::right_turn(p1, p2, legs[j].start())
					|| CGAL::right_turn(p1, p2, legs[j].end()))
				{
					coverages[i][j]--;
					break;
				}
			}
		}
	}

	int start = 0, end = 1, best = n;
	while (end <= n && start <= end) {
		bool covers = true;
		for (int j = 0; j < m - 1 && covers; j++)
			covers = (coverages[end][j] - coverages[start][j]) >= 1;
		
		if (covers && end - start < best)
			best = end - start;

		if (covers)
			start++;
		else
			end++;
	}

	std::cout << best << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
