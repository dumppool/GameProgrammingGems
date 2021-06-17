// -----------------------------------------------------------
// KdTreeDemo - scene.h
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#ifndef I_SCENE_H
#define I_SCENE_H

namespace KdTreeDemo {

// -----------------------------------------------------------
// Vertex class definition
// -----------------------------------------------------------

class Vertex
{
public:
	Vertex() {};
	Vertex( vector3 a_Pos ) { SetOrig( a_Pos ); }
	const vector3& GetPos() const { return m_Pos; }
	const vector3& GetOrig() const { return m_Orig; }
	void SetPos( const vector3& a_Pos ) { m_Pos = a_Pos; }
	void SetOrig( const vector3& a_Pos ) { m_Orig = a_Pos; }
	void Transform( matrix& a_Mat );
	// member data
	vector3 m_Orig, m_Pos;		// 24
	int dummy1, dummy2;			// 8, total 32
};

// -----------------------------------------------------------
// Primitive class definition
// -----------------------------------------------------------

class Primitive
{
public:
	Primitive() {};
	void Init( Vertex* a_V1, Vertex* a_V2, Vertex* a_V3 );
	~Primitive();
	// triangle primitive methods
	const vector3 GetNormal() const { return m_N; }
	void SetNormal( const vector3& a_N ) { m_N = a_N; }
	Vertex* GetVertex( const unsigned int a_Idx ) const { return m_Vertex[a_Idx]; }
	void SetVertex( const unsigned int a_Idx, Vertex* a_Vertex ) { m_Vertex[a_Idx] = a_Vertex; }
	// data members
	vector3 m_N;				// 12
	Vertex* m_Vertex[3];		// 12
	int dummy1, dummy2;			// 8, total 32
};

// -----------------------------------------------------------
// Scene class definition
// -----------------------------------------------------------

class KdTree;
class Scene
{
public:
	Scene();
	~Scene();
	static const unsigned int GetNrPrimitives() { return m_Primitives; }
	static const unsigned int GetNrDynamicPrims() { return m_DPrims; }
	static const Primitive* GetPrimitive( unsigned int a_Idx ) { return m_Primitive[a_Idx]; }
	static Primitive** GetPrimArray() { return m_Primitive; }
	static Primitive** GetDPrimArray() { return m_DPrimitive; }
	static const aabb& GetExtends() { return m_Extends; }
	static void SetExtends( aabb a_Box ) { m_Extends = a_Box; }
	static KdTree* GetKdTree() { return m_KdTree; }
	static KdTree* GetDKdTree() { return m_DKdTree; }
	static void AllocPrims( int a_Count ) { m_Primitive = new Primitive*[a_Count]; }
	static void AllocDPrims( int a_Count ) { m_DPrimitive = new Primitive*[a_Count]; }
	static void SetPrimCount( int a_Count ) { m_Primitives = a_Count; }
	static void SetDPrimCount( int a_DCount ) { m_DPrims = a_DCount; }
	static void SetPrim( int a_Idx, Primitive* a_Prim ) { m_Primitive[a_Idx] = a_Prim; }
	static void SetDPrim( int a_Idx, Primitive* a_Prim ) { m_DPrimitive[a_Idx] = a_Prim; }
	static void LoadOBJ( char* filename, vector3& a_Pos, float a_Scale, bool a_Static );
	static bool m_Flip;
private:
	static unsigned int m_Primitives, m_DPrims;
	static Primitive** m_Primitive;
	static Primitive** m_DPrimitive;
	static aabb m_Extends;
	static KdTree* m_KdTree, *m_DKdTree;
};

}; // namespace KdTreeDemo

#endif
