// -----------------------------------------------------------
// KdTreeDemo - kdtree.h
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#ifndef I_KDTREE_H
#define I_KDTREE_H

#include "template.h"

#pragma warning (disable : 4311)
#pragma warning (disable : 4312)

namespace KdTreeDemo {

class Primitive;
class MManager;
class Scene;

// -----------------------------------------------------------
// Object list helper class
// -----------------------------------------------------------

class ObjectList
{
public:
	ObjectList() : m_Primitive( 0 ), m_Next( 0 ) {}
	~ObjectList() { delete m_Next; }
	void SetPrimitive( const Primitive* a_Prim ) { m_Primitive = a_Prim; }
	const Primitive* GetPrimitive() { return m_Primitive; }
	void SetNext( ObjectList* a_Next ) { m_Next = a_Next; }
	ObjectList* GetNext() { return m_Next; }
	int GetSize() { if (m_Next) return 1 + m_Next->GetSize(); else return 1; }
private:
	const Primitive* m_Primitive;
	ObjectList* m_Next;
};

// -----------------------------------------------------------
// KdTree class definition
// -----------------------------------------------------------

class KdTreeNode
{
public:
	KdTreeNode() : m_Data( 6 ) {};
	void SetAxis( int a_Axis ) { m_Data = (m_Data & -4) + a_Axis; }
	int GetAxis(){ return m_Data & 3; }
	void SetLeft( KdTreeNode* a_Left ) { m_Data = (unsigned long)a_Left + (m_Data & 7); }
	KdTreeNode* GetLeft(){ return (KdTreeNode*)(m_Data & -8); }
	KdTreeNode* GetRight(){ return GetLeft() + 1; }
	int IsLeaf() { return (m_Data & 4); }
	void SetLeaf( bool a_Leaf ) { m_Data = (a_Leaf)?(m_Data|4):(m_Data & -5); }
	int GetObjOffset() { return (m_Data >> 8); }
	int GetObjCount() { return (m_Data & 248) >> 3; }
	void SetSplitPos( float a_Pos ) { m_Split = a_Pos; }
	float GetSplitPos() { return m_Split; }
	void Add( Primitive* a_Prim );
	ObjectList* GetList() { return (ObjectList*)(m_Data&0xfffffff8); }
	void SetList( ObjectList* a_List ) { m_Data = (unsigned long)a_List + (m_Data & 7); }
	void SetObjList( int a_Offs, int a_Count ) { m_Data = (a_Offs << 8) + (a_Count << 3) + (m_Data & 7); }
	// member data
	float m_Split;
	unsigned long m_Data;
};

class PrimData
{
	// struct _aabb { vector3 p1; vector3 p2; };
public:
	PrimData();
	PrimData( Primitive* a_Prim );
	PrimData( PrimData* a_PD );
	void Init( const Primitive* a_Prim );
	bool Clip( float a_Pos, float a_Dir, int a_Axis );
	bool RebuildAndClip( aabb& a_Box );
	void UpdateBBox();
	// data members
	static vector3* s_TVert;
	int m_Verts;
	Primitive* m_Prim;
	vector3* m_Vertex;
	aabb bbox;
};

struct ebox
{
	ebox* next( int a ) { return (ebox*)(n[a] & (0xffffffff - 3)); }
	void next( int a, ebox* p ) { n[a] = (n[a] & 3) + (unsigned long)p; }
	int side( int a ) { return n[a] & 3; }
	void side( int a, int s ) { n[a] = (n[a] & (0xffffffff - 3)) + s; }
	unsigned long n[3];	// 12
	float pos[3];		// 12
};

struct EBox 
{ 
	enum { END, PLANAR, INVALID, START }; 
	enum { LEFTLIST, RIGHTLIST, PROCESSED, STRADDLING }; 
	ebox side[2];			// 48 
	Primitive* prim;		// 4 
	unsigned int flags;     // 4 
	EBox* clone;			// 4 
	int dummy;				// 4, total = 64 
}; 

class EBoxList 
{ 
public: 
	EBoxList() { head[0] = head[1] = head[2] = tail[0] = tail[1] = tail[2] = 0; } 
	void init() { tail[0] = tail[1] = tail[2] = 0; } 
	void sort( int a ); 
	ebox* sort( int a, ebox* first ); 
	void insert( int a, ebox* n ); 
	void remove( int a, ebox* n ); 
	ebox* head[3], *tail[3];
}; 

class KdTree
{
public:
	KdTree();
	~KdTree();
	void InitBuild( Primitive** a_Prim, unsigned int a_PCount );
	void Build( Primitive** a_Prim, unsigned int a_PCount );
	KdTreeNode* GetRoot() const { return m_Root; }
	void SetRoot( KdTreeNode* a_Root ) { m_Root = a_Root; }
	Primitive** GetObjectList() const { return m_ObjList; }
	// tree generation
	int CountLeafPrims( KdTreeNode* a_Node );
	void BuildObjectList( KdTreeNode* a_Node, int a_Depth );
	void BuildTree( ObjectList* a_List, KdTreeNode* a_Node, aabb& a_Box, int a_Depth, int a_Prims );
	void SubdivNewONlogN( KdTreeNode* a_Node, aabb& a_Box, int a_Depth, int a_Prims );
	KdTreeNode* m_Root;
	Primitive** m_ObjList;
	int m_CurObj, m_TravCost, m_IntrCost, m_MaxDepth, m_PPerLeaf;
	bool m_FirstBuild, m_Fast, m_UseFlat;
	ObjectList* m_OList;
};

}; // namespace KdTreeDemo

#endif