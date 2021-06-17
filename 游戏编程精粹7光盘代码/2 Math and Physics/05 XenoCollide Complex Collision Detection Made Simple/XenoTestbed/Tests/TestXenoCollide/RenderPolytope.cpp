
#include "StdAfx.h"

#include <stdio.h>
#include <gl/gl.h>
#include <map>

#include "DrawUtil.h"
#include "HullMaker.h"
#include "ListVector.h"
#include "MathUtil.h"
#include "RenderPolytope.h"
#include "ShrinkWrap.h"

using namespace std;

extern bool gCullFrontFace;

RenderPolytope::RenderPolytope() : mFaces(NULL), mVerts(NULL), mFaceCount(0), mVertCount(0), mListValid(false)
{
}

RenderPolytope::~RenderPolytope()
{
	int32 i;

	for (i=0; i < mFaceCount; i++)
	{
		delete[] mFaces[i].mVertList;
	}
	delete[] mFaces;
	mFaces = NULL;

	delete[] mVerts;
	mVerts = NULL;

	if (mListValid)
	{
		glDeleteLists(mDrawList, 1);
		mListValid = false;
	}
}

void RenderPolytope::Init(CollideGeometry& geom, int32 level)
{
	HullMaker hullMaker;
	ShrinkWrap(&hullMaker, geom, level);
	Init(hullMaker);
}

void RenderPolytope::Init(HullMaker& hullMaker)
{
	mVertCount = (int32) hullMaker.m_vertCount;
	mFaceCount = (int32) hullMaker.m_faceCount;

	mVerts = new Vector[ mVertCount ];
	mFaces = new RenderFace[ mFaceCount ];

	map< ListVector, int32 > indexOfVert;

	int32 i = 0;

	while (!hullMaker.m_faceEdges.empty())
	{
		HullMaker::EdgeList faceEdges;
		HullMaker::Edge* e = hullMaker.m_faceEdges.front();
		Vector n (e->n1.X(), e->n1.Y(), e->n1.Z());
		int32 id = e->id1;
		hullMaker.m_faceEdges.pop_front();
		faceEdges.push_back(e);

		while (!hullMaker.m_faceEdges.empty() && hullMaker.m_faceEdges.front()->id1 == id)
		{
			e = hullMaker.m_faceEdges.front();
			hullMaker.m_faceEdges.pop_front();
			faceEdges.push_back(e);
		}

		mFaces[i].mNormal = n;
		mFaces[i].mVertList = new int32[faceEdges.size()];
		mFaces[i].mVertCount = (int32) faceEdges.size();

		int32 j = 0;

		while (!faceEdges.empty())
		{
			HullMaker::Edge* e = faceEdges.front();
			faceEdges.pop_front();

			Vector p(e->p1.X(), e->p1.Y(), e->p1.Z());
			delete e;

			if (indexOfVert.find(p) == indexOfVert.end())
			{
				mVerts[indexOfVert.size()] = p;
				indexOfVert[p] = (int32) indexOfVert.size();
			}
			mFaces[i].mVertList[j++] = indexOfVert[p];
		}

		i++;
	}

	mColor = Vector( RandFloat(0.5f, 1.0f), RandFloat(0.5f, 1.0f), RandFloat(0.5f, 1.0f) );
//	mColor = Vector( 0.5f, 0.5f, 0.5f );
}

void RenderPolytope::Draw(const Quat& q, const Vector& x)
{
	glPushMatrix();
	SetTransform(x, q);
	glColor4f( mColor.X(), mColor.Y(), mColor.Z(), 1.0f );

	if (mListValid)
	{
		glCallList(mDrawList);
	}
	else
	{
		mDrawList = glGenLists(1);
		mListValid = true;

		glNewList(mDrawList, GL_COMPILE_AND_EXECUTE);

		if (gCullFrontFace)
		{
			glCullFace(GL_FRONT);
		}

		for (int32 i=0; i < mFaceCount; i++)
		{
			glBegin(GL_POLYGON);

			glNormal3f(mFaces[i].mNormal.X(), mFaces[i].mNormal.Y(), mFaces[i].mNormal.Z());

			for (int32 j=0; j < mFaces[i].mVertCount; j++)
			{
				Vector v = mVerts[mFaces[i].mVertList[j]];
				glVertex3f(v.X(), v.Y(), v.Z());
			}

			glEnd();
		}

		if (gCullFrontFace)
		{
			glEnable(GL_BLEND);
			glCullFace(GL_BACK);
			glColor4f( mColor.X(), mColor.Y(), mColor.Z(), 0.5f );
		}

		for (int32 i=0; i < mFaceCount; i++)
		{
			glBegin(GL_POLYGON);

			glNormal3f(mFaces[i].mNormal.X(), mFaces[i].mNormal.Y(), mFaces[i].mNormal.Z());

			for (int32 j=0; j < mFaces[i].mVertCount; j++)
			{
				Vector v = mVerts[mFaces[i].mVertList[j]];
				glVertex3f(v.X(), v.Y(), v.Z());
			}

			glEnd();
		}

		glDisable(GL_BLEND);

		glEndList();
	}

	glPopMatrix();
}

void RenderPolytope::SetColor(const Vector& color)
{
	mColor = color;
	if (mListValid)
	{
		glDeleteLists(mDrawList, 1);
		mListValid = false;
	}
}
