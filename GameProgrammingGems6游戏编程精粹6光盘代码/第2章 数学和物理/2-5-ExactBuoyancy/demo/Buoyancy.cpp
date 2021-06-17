/*
 * Copyright (c) 2005 Erin Catto http://www.gphysics.com
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies.
 * Erin Catto makes no representations about the suitability 
 * of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 */

#include "buoyancy.h"
#include <assert.h>

// Returns the volume of a tetrahedron and updates the centroid accumulator.
static float TetrahedronVolume(Vec3& c, Vec3 p, Vec3 v1, Vec3 v2, Vec3 v3)
{
	Vec3 a = v2 - v1;
	Vec3 b = v3 - v1;
	Vec3 r = p - v1;

	float volume = (1.0f/6.0f)*(b % a) * r;
	c += 0.25f*volume*(v1 + v2 + v3 + p);
	return volume;
}

// Clips a partially submerged triangle and returns the volume of the
// resulting tetrahedrons and updates the centroid accumulator.
static float ClipTriangle(	Vec3& c, Vec3 p,
							Vec3 v1, Vec3 v2, Vec3 v3,
							float d1, float d2, float d3)
{
	assert(d1*d2 < 0);
	Vec3 vc1 = v1 + (d1/(d1 - d2))*(v2 - v1);
	float volume = 0;

	if (d1 < 0)
	{
		if (d3 < 0)
		{
			// Case B - a quadrilateral or two triangles.
			Vec3 vc2 = v2 + (d2/(d2 - d3))*(v3 - v2);
			volume += TetrahedronVolume(c, p, vc1, vc2, v1);
			volume += TetrahedronVolume(c, p, vc2, v3, v1);
		}
		else
		{
			// Case A - a single triangle.
			Vec3 vc2 = v1 + (d1/(d1 - d3))*(v3 - v1);
			volume += TetrahedronVolume(c, p, vc1, vc2, v1);
		}
	}
	else
	{
		if (d3 < 0)
		{
			// Case B
			Vec3 vc2 = v1 + (d1/(d1 - d3))*(v3 - v1);
			volume += TetrahedronVolume(c, p, vc1, v2, v3);
			volume += TetrahedronVolume(c, p, vc1, v3, vc2);
		}
		else
		{
			// Case A
			Vec3 vc2 = v2 + (d2/(d2 - d3))*(v3 - v2);
			volume += TetrahedronVolume(c, p, vc1, v2, vc2);
		}
	}

	return volume;
}

// Computes the submerged volume and center of buoyancy of a polyhedron with
// the water surface defined as a plane.
static float SubmergedVolume(Vec3& c, Vec3 x, Quaternion q,
							Polyhedron& poly, Plane& plane)
{
	// Transform the plane into the polyhedron frame.
	Quaternion qt = q.Conjugate();
	Vec3 normal = qt.Rotate(plane.normal);
	float offset = plane.offset - plane.normal*x;

	// Compute the vertex heights relative to the surface.
	float TINY_DEPTH = -1e-6f;
	float* ds = new float [poly.numVerts];

	// Compute the depth of each vertex.
	int numSubmerged = 0;
	int sampleVert = 0;
	for (int i = 0; i < poly.numVerts; ++i)
	{
		ds[i] = normal*poly.verts[i] - offset;
		if (ds[i] < TINY_DEPTH)
		{
			++numSubmerged;
			sampleVert = i;
		}
	}

	// Return early if no vertices are submerged
	if (numSubmerged == 0)
	{
		c.SetZero();
		delete [] ds;
		return 0;
	}

	// Find a point on the water surface. Project a submerged point to
	// get improved accuracy. This point serves as the point of origin for
	// computing all the tetrahedron volumes. Since this point is on the
	// surface, all of the surface faces get zero volume tetrahedrons. This
	// way the surface polygon does not need to be considered.
	Vec3 p = poly.verts[sampleVert] - ds[sampleVert]*normal;

	// Initialize volume and centroid accumulators.
	float volume = 0;
	c.SetZero();

	// Compute the contribution of each triangle.
	for (int i = 0; i < poly.numFaces; ++i)
	{
		int i1 = poly.faces[i].i1;
		int i2 = poly.faces[i].i2;
		int i3 = poly.faces[i].i3;

		Vec3 v1 = poly.verts[i1];
		float d1 = ds[i1];

		Vec3 v2 = poly.verts[i2];
		float d2 = ds[i2];

		Vec3 v3 = poly.verts[i3];
		float d3 = ds[i3];

		if (d1 * d2 < 0)
		{
			// v1-v2 crosses the plane
			volume += ClipTriangle(c, p, v1, v2, v3, d1, d2, d3);
		}
		else if (d1 * d3 < 0)
		{
			// v1-v3 crosses the plane
			volume += ClipTriangle(c, p, v3, v1, v2, d3, d1, d2);
		}
		else if (d2 * d3 < 0)
		{
			// v2-v3 crosses the plane
			volume += ClipTriangle(c, p, v2, v3, v1, d2, d3, d1);
		}
		else if (d1 < 0 || d2 < 0 || d3 < 0)
		{
			// fully submerged
			volume += TetrahedronVolume(c, p, v1, v2, v3);
		}
	}

	// Small submerged slivers may have roundoff error leading to a zero or negative
	// volume. If so, then return a result of zero.
	float TINY_VOLUME = 1e-6f;
	if (volume <= TINY_VOLUME)
	{
		c.SetZero();
		delete [] ds;
		return 0;
	}

	// Normalize the centroid by the total volume.
	c *= 1.0f/volume;

	// Transform the centroid into world coordinates.
	c = x + q.Rotate(c);

	delete [] ds;
	return volume;
}

float ComputeVolume(Polyhedron& poly)
{
	float volume = 0;
	Vec3 c, zero;
	c.SetZero();
	zero.SetZero();

	// Compute the contribution of each triangle.
	for (int i = 0; i < poly.numFaces; ++i)
	{
		int i1 = poly.faces[i].i1;
		int i2 = poly.faces[i].i2;
		int i3 = poly.faces[i].i3;

		Vec3 v1 = poly.verts[i1];
		Vec3 v2 = poly.verts[i2];
		Vec3 v3 = poly.verts[i3];

		volume += TetrahedronVolume(c, zero, v1, v2, v3);
	}

	return volume;
}

// Compute the buoyancy and drag forces.
void ComputeBuoyancy(RigidBody& body, Polyhedron& poly,
					 WaterVolume& water, float gravity)
{
	Vec3 c;
	float volume = SubmergedVolume(c, body.x, body.q, poly, water.plane);

	if (volume > 0)
	{
		Vec3 buoyancyForce = (water.density*volume*gravity)*water.plane.normal;

		float partialMass = body.mass * volume / poly.volume;
		Vec3 rc = c - body.x;
		Vec3 vc = body.v + body.omega % rc;
		Vec3 dragForce = (partialMass*water.linearDrag)*(water.velocity - vc);

		Vec3 totalForce = buoyancyForce + dragForce;
		body.F += totalForce;
		body.T += rc % totalForce;

		float length2 = poly.length*poly.length;
		Vec3 dragTorque = (-partialMass*water.angularDrag*length2)*body.omega;
		body.T += dragTorque;
	}
}
