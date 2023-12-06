#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using P = K::Point_2;
using R = K::Ray_2;

struct Entry {
	long y;
	int i;
	R r;
};

class StartPos {
public:
	bool operator() (const Entry &o1, const Entry &o2) const {
		return o1.y < o2.y;
	}
};

class Index {
public:
	bool operator() (const Entry &o1, const Entry &o2) const {
		return o1.i < o2.i;
	}
};

void testcase() {
	int n; std::cin >> n;
	P o1(0, 0), o2(0, 1);
	std::vector<Entry> r(n);
	std::vector<Entry> s;
	s.reserve(n);

	for (int i = 0; i < n; i++) {
		long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
		P p1(0, y0), p2(x1, y1);
		r[i] = Entry({y0, i, R(p1, p2)});
	}

	std::sort(r.begin(), r.end(), StartPos());

	for (auto i = r.begin(); i != r.end(); i++) {
		bool add = true;
		
		for (auto j = s.rbegin(); j != s.rend(); j++) {
			auto intersect = CGAL::intersection(i->r, j->r);

			if (!intersect)
				break;
			else if (const P* itx = boost::get<P>(&*intersect)) {
				if (CGAL::left_turn(o1, o2, *itx))
					break;

				if (CGAL::squared_distance(*itx, i->r.start()) >= CGAL::squared_distance(*itx, j->r.start())) {
					add = false;
					break;
				} else
					s.pop_back();
			}
		}

		if (add)
			s.push_back(*i);
	}

	std::sort(s.begin(), s.end(), Index());
	for (const auto& e: s)
		std::cout << e.i << " ";
	
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
