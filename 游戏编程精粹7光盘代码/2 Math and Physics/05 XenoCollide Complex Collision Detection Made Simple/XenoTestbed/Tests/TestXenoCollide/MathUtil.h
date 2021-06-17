#pragma once

class Body;
class RenderPolytope;

// Return a random floating-point value in the range [min, max]
float32 RandFloat(float32 min, float32 max);

// Compute the mass properties (mass, center of mass, inertia tensor and inverse masses) using
// a RenderPolytope as an approximation of the body shape.
void ComputeMassProperties(Body* body, RenderPolytope* model, float32 density);
