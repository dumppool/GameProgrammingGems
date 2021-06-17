#pragma once

// Set OpenGL's model-view transform
void SetTransform(const Vector& pos, const Quat& q);

// Draw a sphere using OpenGL
void DrawSphere(const Vector& pos, const Quat& q, float radius, const Vector& c);

