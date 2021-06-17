
#include "StdAfx.h"

#include "MathUtil.h"
#include "RigidBody.h"

extern float32 gColorMin;

RigidBody::RigidBody(Body* b, CollideGeometry* cg, RenderPolytope* rp, float32 radius) : body(b), collideModel(cg), renderModel(rp), maxRadius(radius)
{
	color = Vector( RandFloat(gColorMin, 1.0f), RandFloat(gColorMin, 1.0f), RandFloat(gColorMin, 1.0f) );
}

