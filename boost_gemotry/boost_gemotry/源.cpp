#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <iostream>
#include <vector>
namespace bg = boost::geometry;
using point = bg::model::point<double, 2, bg::cs::cartesian>;
using polygon = bg::model::polygon<point>;
using box = bg::model::box<point>;
using value = std::pair<box, std::size_t>;
using rtree = bg::index::rtree<value, bg::index::rstar<16>>;
int main() {
	std::vector<polygon> polys(4);
	bg::read_wkt("POLYGON((0 0, 0 1, 1 0, 0 0))", polys[0]);
	bg::read_wkt("POLYGON((1 1, 1 2, 2 1, 1 1))", polys[1]);
	bg::read_wkt("POLYGON((2 2, 2 3, 3 2, 2 2))", polys[2]);
	bg::read_wkt("POLYGON((3 3, 3 4, 4 3, 3 3))", polys[3]);
	rtree rt;
	for (std::size_t i = 0; i < polys.size(); ++i) {
		box b = bg::return_envelope<box>(polys[i]);
		rt.insert(std::make_pair(b, i));
	}
	polygon qpoly;
	bg::read_wkt("POLYGON((0.25 0.25,0.5 1.5,0.9 0.9,1.5 0.5,0.25 0.25))",
		qpoly);
	box qbox = bg::return_buffer<box>(bg::return_envelope<box>(qpoly),
		0.0001);
	std::vector<value> result;
	rt.query(bg::index::intersects(qbox), std::back_inserter(result));
	for (value const& v : result) {
		std::cout << bg::wkt(polys[v.second])
			<< (bg::intersects(polys[v.second], qpoly)
				? " intersects" : " not intersects") << std::endl;
	}
}