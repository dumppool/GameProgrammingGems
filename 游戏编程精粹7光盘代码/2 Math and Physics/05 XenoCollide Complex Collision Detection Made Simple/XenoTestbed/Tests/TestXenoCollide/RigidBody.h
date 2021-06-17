#pragma once

#include "CollideGeometry.h"
#include "Constraint.h"
#include "MapPtr.h"
#include "RenderPolytope.h"

//////////////////////////////////////////////////////////////////////////////
// RigidBody
//
// These are the objects you see in the physics simulation within the demo.
// Each RigidBody object contains a physics model (Body), a collision model
// (CollideGeometry), and render model (RenderPolytope) and a unique color.
//
// The physics and render models are created directly from the collision
// model, which itself is a support mapping that represents the shape.

class RigidBody
{

public:

	RigidBody(Body* b, CollideGeometry* cg, RenderPolytope* rp, float32 radius);

	Vector						color;
	MapPtr<Body>				body;
	MapPtr<CollideGeometry>		collideModel;
	MapPtr<RenderPolytope>		renderModel;
	float32						maxRadius;

};

