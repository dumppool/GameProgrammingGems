
#pragma once

#include <list>
#include <stack>
#include <vector>

#include "Math/Math.h"

#include "ListVector.h"

//////////////////////////////////////////////////////////////////////////////
// HullMaker is a utility class.
//
// It take a set of points and generates a set of faces that form a convex
// hull around the points.  This allows us to render the shape.

class HullMaker
{

public:

	class Edge;

	typedef std::list<Edge*> EdgeList;
	typedef std::list<ListVector> PointList;

	typedef std::vector<Edge*> EdgeVector;

	class Edge
	{
	public:

		Edge(const Vector& _p1, const Vector& _p2, const Vector& _n1, const Vector& _n2, const Vector& _np1, const Vector& _np2, int32 _id1, int32 _id2)
			: p1(_p1), p2(_p2), n1(_n1), n2(_n2), np1(_np1), np2(_np2), id1(_id1), id2(_id2) {}
		Vector p1;
		Vector p2;
		Vector n1;
		Vector n2;
		Vector np1;
		Vector np2;
		int32 id1;
		int32 id2;
	};

	class TreeNode
	{
	public:
		TreeNode();
		void AddPoint( const Vector& p );
		void SplitNode( TreeNode* child1, TreeNode* child2, const Vector& n, const Vector& p );
		Vector minBounds;
		Vector maxBounds;
		std::list<ListVector> pointList;
		EdgeList outsideEdgeList;
	};

	TreeNode* m_points;
	EdgeList m_edgeList;
	EdgeVector m_sortedEdges;
	EdgeList m_faceEdges;
	Vector m_p0;
	Vector m_p1;
	int32 m_vertCount;
	int32 m_faceCount;
	int32 m_faceId;
	std::stack<TreeNode*> m_pointStack;

	HullMaker();
	~HullMaker();
	void Clear();
	void AddPoint(const Vector& p);
	void AddPointToHull(const Vector& p);
	bool AddInitialVert(const Vector& p);
	bool CreateHull();
	void ProcessTreeNode(TreeNode* node);
};
