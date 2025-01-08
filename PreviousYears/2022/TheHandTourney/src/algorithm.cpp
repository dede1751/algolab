#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef K::Point_2 P;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<P,Index> IPoint;

long n, k, f0, s0;

bool solves(long f, const long s, EdgeV& edges) {
	boost::disjoint_sets_with_storage<> uf(n);
	long n_components = n;
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		if (std::get<2>(*e) >= s)
			break;

		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));

		if (c1 != c2) {
			uf.link(c1, c2);
			if (--n_components == 1) break;
		}
	}

	if (k == 1)
		return f <= n_components;

	std::vector<long> counter(n, 0);
	std::vector<long> bins(5, 0);
	for (int i = 0; i < n; i++) {
		int idx = uf.find_set(i);
		counter[uf.find_set(i)]++;
	}

	for (int i = 0; i < n_components; i++)
		bins[std::min(counter[i], (long)4)]++;
	
	if (k == 2)
		return f <= bins[1] / 2 + bins[2] + bins[3] + bins[4];
	else if (k == 3) {
		if (bins[1] >= bins[2])
			return f <= (bins[1] - bins[2]) / 3 + bins[2] + bins[3] + bins[4];
		else
			return f <= bins[1] + bins[2] / 2 + bins[3] + bins[4];
	} else {
		if (bins[1] == bins[3])
			return f <= bins[2] / 2 + bins[3] + bins[4];
		else if (bins[1] > bins[3]) {
			if (bins[2] % 2 && bins[1] >= 2)
				return f <= 1 + (bins[1] - 2 - bins[3]) / 4 + bins[2] / 2 + bins[3] + bins[4];
			else
				return f <= (bins[1] - bins[3]) / 4 + bins[2] / 2 + bins[3] + bins[4];
		} else
			return f <= bins[1] + bins[2] / 2 + (bins[3] + (bins[2] % 2) - bins[1]) / 2 + bins[4];
	}
}

void testcase() {
	std::cin >> n >> k >> f0 >> s0;
	std::vector<IPoint> points(n);

	for (Index i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		points[i] = {P(x, y), i};
	}

	Delaunay t;
	t.insert(points.begin(), points.end());

	EdgeV edges;
	edges.reserve(3*n);
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		if (i1 > i2) std::swap(i1, i2);
		edges.emplace_back(i1, i2, t.segment(e).squared_length());
	}

	std::sort(edges.begin(), edges.end(),
		[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
	});

	long start = 1, end = ((long)1<<50) + 1;
	while (start < end) {
		long mid = start + (end - start) / 2;

		if (solves(f0, mid, edges))
			start = mid + 1;
		else
			end = mid;
	}

	std::cout << start - 1 << " ";

	start = 0;
	end = n + 1;
	while (start < end) {
		long mid = start + (end - start) / 2;

		if (solves(mid, s0, edges))
			start = mid + 1;
		else
			end = mid;
	}

	std::cout << start - 1 << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
