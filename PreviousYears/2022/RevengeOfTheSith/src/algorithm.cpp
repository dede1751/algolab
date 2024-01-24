#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

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

long n, r;

int rebels(std::vector<IPoint>& points, const int k) {
	Delaunay t;
	t.insert(points.begin() + k, points.end());

	EdgeV edges;
	edges.reserve(3*n);
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();

		if (i1 > i2) std::swap(i1, i2);
		if (t.segment(e).squared_length() <= r * r)
			edges.emplace_back(i1, i2, t.segment(e).squared_length());
	}

	std::sort(edges.begin(), edges.end(),
		[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
	});

	boost::disjoint_sets_with_storage<> uf(n);
	Index n_components = n;
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
			Index c1 = uf.find_set(std::get<0>(*e));
			Index c2 = uf.find_set(std::get<1>(*e));
			if (c1 != c2) {
			uf.link(c1, c2);
			if (--n_components == 1) break;
		}
	}

	std::vector<int> occs(n, 0);
	for (int i = k; i < n; i++)
		occs[uf.find_set(i)]++;

	return std::min(k, *std::max_element(occs.begin(), occs.end()));
}

void testcase() {
	std::cin >> n >> r;

	std::vector<IPoint> points(n);
	for (Index i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		points[i] = {P(x, y), i};
	}

	int start = 1, end = (n / 2) + 1;
	while (start < end) {
		int mid = start + (end - start) / 2;
		int r1 = rebels(points, mid), r2 = rebels(points, mid + 1);

		if (r1 < r2)
			start = mid + 1;
		else
			end = mid;
	}

	std::cout << start << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
