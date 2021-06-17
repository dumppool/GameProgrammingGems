#pragma once

#include "Math/Math.h"

//////////////////////////////////////////////////////////////////////////////
// ListVector is a helper class.
//
// Vector objects must be aligned to 16 bytes.  Unfortunately, the STL
// containers don't guarantee 16-byte alignment.  ListVector is a simple
// data structure that can hold the same data used by Vector, but doesn't
// require 16-byte alignment.  ListVectors are used whenever Vectors need
// to be stored in an STL container.

class ListVector
{

public:

	ListVector(const Vector& v) { *this = v; }
	ListVector operator= (const Vector& v) { x = v.X(); y = v.Y(); z = v.Z(); return *this; }
	operator Vector () { return Vector(x, y, z); }
	float x;
	float y;
	float z;
};

inline bool operator < (const ListVector& a, const ListVector& b)
{
	if (a.x < b.x) return true;
	if (a.x > b.x) return false;
	if (a.y < b.y) return true;
	if (a.y > b.y) return false;
	if (a.z < b.z) return true;
	if (a.z > b.z) return false;
	return false;
}
