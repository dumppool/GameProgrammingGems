#include "StdAfx.h"

#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

void SetTransform(const Vector& pos, const Quat& q)
{
	Matrix transform(q);
	transform.SetTrans(pos);

	glMultMatrixf((GLfloat*) &transform);
}

void DrawSphere(const Vector& pos, const Quat& q, float radius, const Vector& c)
{
	glColor3f(c.X(), c.Y(), c.Z());
	glPushMatrix();
	SetTransform(pos, q);
	GLUquadricObj* quadric = gluNewQuadric();
	gluSphere(quadric, radius, 10, 10);
	gluDeleteQuadric(quadric);
	glPopMatrix();
}

