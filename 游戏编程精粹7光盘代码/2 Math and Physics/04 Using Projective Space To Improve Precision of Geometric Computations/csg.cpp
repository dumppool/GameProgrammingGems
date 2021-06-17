
#include <math.h>

#include "csg.h"


using namespace std;




// **************** PointRP2 ****************

// create line running from this point to the other point
// works only if both points were created by giving
// coordinates explicitly
// (i.e. doesn't work for intersection points)
LineRP2 PointRP2::line_to(const PointRP2 &p) const
{
	return LineRP2(v.cross(p.v));
}



// **************** LineRP2 ****************

// find intersection point of two lines
PointRP2 LineRP2::intersect(const LineRP2 &l) const
{
	PointRP2 out( normal.cross(l.normal) );
	if(out.v.z<0)
		out.v = -out.v;
	return out;
}

// check which side of the line the point lies on:
//	result > 0		- right side
//	result = 0		- on the line
//	result < 0		- left side
coord_t LineRP2::check_side(const PointRP2 &p) const
{
	return normal.dot(p.v);
}



// **************** Polygon2D ****************

// create polygon from list of vertices
// (vertices should be ordered clockwise,
//  should form a convex shape, no three
//  vertices can be colinear and no vertex
//  should be specified twice)
Polygon2D::Polygon2D(PointRP2 *vtx,int n_vtx)
{
	Edge2D e;
	for(int i=0;i<n_vtx;i++)
	{
		e.start = vtx[i];
		e.line = vtx[i].line_to(vtx[(i+1)%n_vtx]);
		edges.push_back(e);
	}
	update_bounding_box();
}

// split polygon with a line,
// resulting polygons can be empty if no part
// of initial polygon lies on given side,
void Polygon2D::split_by_line(const LineRP2 &line,Polygon2D &left,Polygon2D &right) const
{
	int n_left = 0, n_right = 0;
	Edge2D e;

	// clear output polygons, in case they weren't empty
	left.edges.clear();
	left.update_bounding_box();

	right.edges.clear();
	right.update_bounding_box();

	if(is_empty())
		return;

	// check if polygon lies completely on one side of the line
	for(unsigned int i=0;i<edges.size();i++)
	{
		coord_t s = line.check_side(edges[i].start);
		if(s>0) n_right++;
		if(s<0) n_left++;
	}

	if(n_left==0)
	{
		right = *this;
		return;
	}

	if(n_right==0)
	{
		left = *this;
		return;
	}

	// assign edges to output polygons and split them when neccessary,
	// edges closing polygons are thrown in when edge crossing the line is detected
	// (crossing left->right closes the left polygon and crossing right->left closes the right one)

	for(unsigned int i=0;i<edges.size();i++)
	{
		coord_t s1 = line.check_side(edges[i].start);
		coord_t s2 = line.check_side(edges[(i+1)%edges.size()].start);

		if(s1<0 && s2>0)
		{
			PointRP2 mid = line.intersect(edges[i].line);

			// part of the edge on left side
			e.start = edges[i].start;
			e.line = edges[i].line;
			left.edges.push_back(e);

			// midpoint is start of the edge closing left polygon
			e.start = mid;
			e.line = line.reversed();
			left.edges.push_back(e);

			// part of the edge on right side
			e.start = mid;
			e.line = edges[i].line;
			right.edges.push_back(e);
		}
		else if(s1>0 && s2<0)
		{
			PointRP2 mid = line.intersect(edges[i].line);

			// part of the edge on right side
			e.start = edges[i].start;
			e.line = edges[i].line;
			right.edges.push_back(e);

			// midpoint is start of the edge closing right polygon
			e.start = mid;
			e.line = line;
			right.edges.push_back(e);

			// part of the edge on left side
			e.start = mid;
			e.line = edges[i].line;
			left.edges.push_back(e);
		}
		else if(s1<0 || s2<0)
		{
			// edge is completely of the left side
			left.edges.push_back(edges[i]);

			if(s1==0)
			{
				// starting point lies on the line, so use it as
				// starting point of edge closing right polygon
				e.start = edges[i].start;
				e.line = line;
				right.edges.push_back(e);
			}
		}
		else if(s1>0 || s2>0)
		{
			// edge is completely on the right side
			right.edges.push_back(edges[i]);

			if(s1==0)
			{
				// starting point lies on the line, so use it as
				// starting point of edge closing left polygon
				e.start = edges[i].start;
				e.line = line.reversed();
				left.edges.push_back(e);
			}
		}
		else
		{
			// remaining case: s1==0 && s2==0
			// this can happen only if initial polygon was incorrect
		}
	}

	// update bounding boxes of resulting polygons
	left.update_bounding_box();
	right.update_bounding_box();
}

// update bounding box of the polygon,
// this function is called automatically
void Polygon2D::update_bounding_box()
{
	if(is_empty())
	{
		box_min_x = box_max_x = 0;
		box_min_y = box_max_y = 0;
		return;
	}

	double min_x =  1e20;
	double min_y =  1e20;
	double max_x = -1e20;
	double max_y = -1e20;

	for(int i=0;i<edges.size();i++)
	{
		double x = edges[i].start.get_x();
		if(x<min_x) min_x = x;
		if(x>max_x) max_x = x;

		double y = edges[i].start.get_y();
		if(y<min_y) min_y = y;
		if(y>max_y) max_y = y;
	}

	box_min_x = int(floor(min_x)) - 1;
	box_min_y = int(floor(min_y)) - 1;
	box_max_x = int(ceil (max_x)) + 1;
	box_max_y = int(ceil (max_y)) + 1;
}


// **************** PolygonMesh2D ****************

// add polygon to the mesh
void PolygonMesh2D::csg_add(const Polygon2D &p)
{
	// make hole for new polygon
	csg_subtract(p);

	// add the polygon
	polys.push_back(p);
}

// make hole in the mesh
void PolygonMesh2D::csg_subtract(const Polygon2D &p)
{
	vector<Polygon2D> result;
	vector<Polygon2D> outA;
	Polygon2D C, left, right;

	for(int i=0;i<polys.size();i++)
	{
		// if bounding boxes don't touch, polygon lies outside
		// of the polygon being subtracted, so add it to the solution
		if(!polys[i].box_collide(p))
		{
			result.push_back(polys[i]);
			continue;
		}

		// init polygon C and set outA
		C = polys[i];
		outA.clear();

		// cur polygon C with edges of polygon being subtracted
		int j = 0;
		while(!C.is_empty() && j<p.edges.size())
		{
			C.split_by_line(p.edges[j].line,left,right);
			C = right;
			if(!left.is_empty())
				outA.push_back(left);

			j++;
		}

		// if C is left empty it means all parts of initial polygon
		// were added to set outA, so add entire polygon instead of
		// set outA to save some splits
		if(C.is_empty())
			result.push_back(polys[i]);
		else
			result.insert(result.end(),outA.begin(),outA.end());
	}

	// replace old mesh with new result
	polys.swap(result);
}
