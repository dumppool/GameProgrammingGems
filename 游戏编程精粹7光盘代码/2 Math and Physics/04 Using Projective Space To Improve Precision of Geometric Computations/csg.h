
#ifndef _CSG_H
#define _CSG_H


#include <vector>



// **************** Vector3D ****************

template<class _T>
class Vector3D {
public:
	_T		x, y, z;

	Vector3D() {}
	Vector3D(_T _x,_T _y,_T _z) : x(_x), y(_y), z(_z) {}

	Vector3D<_T> operator -() const { return Vector3D<_T>(-x,-y,-z); }

	_T dot(const Vector3D<_T> &v) const { return x*v.x + y*v.y + z*v.z; }

	Vector3D<_T> cross(const Vector3D<_T> &v) const
	{
		return Vector3D<_T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
};


typedef __int64		coord_t;





// **************** PointRP2 ****************

class LineRP2;

class PointRP2 {
public:
	Vector3D<coord_t>	v;


	PointRP2() {}
	PointRP2(int xr,int yr) : v(xr,yr,1) {}
	PointRP2(const Vector3D<coord_t> &_v) : v(_v) {}

	LineRP2 line_to(const PointRP2 &p) const;

	double get_x() const { return double(v.x)/v.z; }
	double get_y() const { return double(v.y)/v.z; }
};


// **************** LineRP2 ****************

class LineRP2 {
public:
	Vector3D<coord_t>	normal;


	LineRP2() {}
	LineRP2(const Vector3D<coord_t> &_normal) : normal(_normal) {}

	PointRP2 intersect(const LineRP2 &l) const;
	coord_t  check_side(const PointRP2 &p) const;

	LineRP2  reversed() const { return LineRP2(-normal); }
};




// **************** Edge2D ****************

class Edge2D {
public:
	PointRP2	start;	// start point of next edge is end point
	LineRP2		line;
};


// **************** Polygon2D ****************

class Polygon2D {
public:
	std::vector<Edge2D>	edges;
	int					box_min_x;
	int					box_min_y;
	int					box_max_x;
	int					box_max_y;


	Polygon2D() {}
	Polygon2D(PointRP2 *vtx,int n_vtx);

	void split_by_line(const LineRP2 &line,Polygon2D &left,Polygon2D &right) const;

	void update_bounding_box();

	bool is_empty() const
	{
		return (edges.size()==0);
	}

	bool box_collide(const Polygon2D &other) const
	{
		return	box_min_x<=other.box_max_x &&
				box_max_x>=other.box_min_x &&
				box_min_y<=other.box_max_y &&
				box_max_y>=other.box_min_y;
	}
};


// **************** PolygonMesh2D ****************

class PolygonMesh2D {
public:
	std::vector<Polygon2D>	polys;

	void csg_add(const Polygon2D &p);
	void csg_subtract(const Polygon2D &p);

};



#endif
