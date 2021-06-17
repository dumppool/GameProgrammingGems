
#pragma once

//////////////////////////////////////////////////////////////////////////////
// This file (and its associated *.cpp file) contain the implementation of
// the XenoCollide algorithm.

class CollideGeometry;

//////////////////////////////////////////////////////////////////////////////
// Intersect() is the simplest XenoCollide routine.  It returns true if two
// CollideGeometry objects overlap, or false if they do not.

bool Intersect(CollideGeometry& p1, const Quat& q1, const Vector& t1, CollideGeometry& p2, const Quat& q2, const Vector& t2, float32 boundaryTolerance);

//////////////////////////////////////////////////////////////////////////////
// CollideAndFindPoint() also determines whether or not two CollideGeometry
// objects overlap.  If they do, it finds contact information.

bool CollideAndFindPoint(CollideGeometry& p1, const Quat& q1, const Vector& t1, CollideGeometry& p2, const Quat& q2, const Vector& t2, Vector* returnNormal, Vector* point1, Vector* point2);

//////////////////////////////////////////////////////////////////////////////
// TransformSupportVert() finds the support point for a rotated and/or
// translated CollideGeometry.

Vector TransformSupportVert( CollideGeometry& p, const Quat& q, const Vector& t, const Vector& n );
