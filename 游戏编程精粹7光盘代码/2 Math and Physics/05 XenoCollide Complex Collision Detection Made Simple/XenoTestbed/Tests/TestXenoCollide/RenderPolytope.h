
#pragma once

class CollideGeometry;
class HullMaker;

//////////////////////////////////////////////////////////////////////////////
// RenderPolytope represents a drawable polytope.
//
// You can pass a CollideGeometry to RenderPolytope::Init() to have it create
// a renderable approximation of the shape.

class RenderPolytope
{
public:
	struct RenderFace
	{
		Vector mNormal;
		int32* mVertList;
		int32 mVertCount;
	};

	RenderFace*	mFaces;
	Vector* mVerts;

	int32 mFaceCount;
	int32 mVertCount;

	Vector	mColor;

	bool mListValid;
	uint32 mDrawList;

public:

	RenderPolytope();
	~RenderPolytope();

	void Init(HullMaker& hullMaker);
	void Init(CollideGeometry& geom, int32 level = -1);
	void Draw(const Quat& q, const Vector& x);
	void SetColor(const Vector& color);
};

