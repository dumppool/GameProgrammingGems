// -----------------------------------------------------------
// KdTreeDemo - scene.cpp
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#include "template.h"
#include "scene.h"
#include "kdtree.h"

namespace KdTreeDemo {

aabb Scene::m_Extends;
unsigned int Scene::m_Primitives, Scene::m_DPrims;
bool Scene::m_Flip;
Primitive** Scene::m_Primitive, **Scene::m_DPrimitive;
KdTree* Scene::m_KdTree, *Scene::m_DKdTree;

// -----------------------------------------------------------
// Primitive methods
// -----------------------------------------------------------

void Primitive::Init( Vertex* a_V1, Vertex* a_V2, Vertex* a_V3 )
{
	m_Vertex[0] = a_V1, m_Vertex[1] = a_V2,	m_Vertex[2] = a_V3;
	// calculate normal
	vector3 c = a_V2->GetOrig() - a_V1->GetOrig();
	vector3 b = a_V3->GetOrig() - a_V1->GetOrig();
	m_N = b.Cross( c );
	m_N.Normalize();
}

// -----------------------------------------------------------
// Vertex class implementation
// -----------------------------------------------------------
void Vertex::Transform( matrix& a_Mat )
{
	m_Pos = a_Mat.Transform( m_Orig );
}

// -----------------------------------------------------------
// Scene class implementation
// -----------------------------------------------------------
Scene::Scene()
{
	m_Primitives = 0;
	m_Primitive = 0;
	m_DPrimitive = 0;
	m_DPrims = 0;
	vector3 a( 0, 0, 0 );
	m_Extends = aabb( a, a );
	m_KdTree = new KdTree();
	m_DKdTree = new KdTree();
}

Scene::~Scene()
{
	delete m_Primitive;
	delete m_DPrimitive;
}

void Scene::LoadOBJ( char* filename, vector3& a_Pos, float a_Scale, bool a_Static )
{
	// count faces in file
	FILE* f = fopen( filename, "r" );
	if (!f) return;
	unsigned int fcount = 0, vcount = 0;
	char buffer[256], cmd[32];
	while (1)
	{
		fgets( buffer, 250, f );
		if (feof( f )) break;
		if (buffer[0] == 'v') 
		{
			if (buffer[1] == ' ') vcount++;
		}
		else if ((buffer[0] == 'f') && (buffer[1] == ' ')) fcount++;
	}
	fclose( f );
	Primitive** primarray;
	if (a_Static) primarray = m_Primitive = new Primitive*[fcount + 64];
			 else primarray = m_DPrimitive = new Primitive*[fcount + 64];
	f = fopen( filename, "r" );
	vector3* vert = new vector3[vcount];
	unsigned int verts = 0;
	unsigned long addr = (unsigned long)(char*)(new Vertex[fcount * 3 + 4]);
	Vertex* vertex = (Vertex*)((addr + 32) & (0xffffffff - 31)); 
	unsigned int vidx = 0;
	// load data
	unsigned int primidx = 0;
	while (1)
	{
		fgets( buffer, 250, f );
		if (feof( f )) break;
		switch (buffer[0])
		{
		case 'f': // face
			{
				Vertex* v[3];
				unsigned int vnr[9];
				unsigned int vars = sscanf( buffer + 2, "%i/%i/%i %i/%i/%i %i/%i/%i", 
					&vnr[0], &vnr[1], &vnr[2], &vnr[3], &vnr[4], &vnr[5], &vnr[6], &vnr[7], &vnr[8] );
				if (vars < 9) 
				{
					vars = sscanf( buffer + 2, "%i/%i %i/%i %i/%i", &vnr[0], &vnr[2], &vnr[3], &vnr[5], &vnr[6], &vnr[8] );
					if (vars < 6) sscanf( buffer + 2, "%i//%i %i//%i %i//%i", &vnr[0], &vnr[2], &vnr[3], &vnr[5], &vnr[6], &vnr[8] );
				}
				for ( unsigned int i = 0; i < 3; i++ )
				{
					v[i] = &vertex[vidx++];
					v[i]->SetPos( vert[vnr[i * 3] - 1] );
					v[i]->SetOrig( vert[vnr[i * 3] - 1] );
				}
				Primitive* p = new Primitive();
				if (a_Static) primarray[m_Primitives++] = p; else primarray[m_DPrims++] = p;
				if (m_Flip) p->Init( v[2], v[1], v[0] ); else p->Init( v[0], v[1], v[2] );
			}
			break;
		case 'v': // vertex
			if (buffer[1] == ' ')
			{
				float x, y, z;
				sscanf( buffer, "%s %f %f %f", cmd, &x, &y, &z );
				vector3 pos = vert[verts++] = vector3( x * a_Scale, y * a_Scale, z * a_Scale ) + a_Pos;
				for ( unsigned int a = 0; a < 3; a++ )
				{
					if (pos.cell[a] < m_Extends.GetP1().cell[a]) m_Extends.GetP1().cell[a] = pos.cell[a];
					if (pos.cell[a] > m_Extends.GetP2().cell[a]) m_Extends.GetP2().cell[a] = pos.cell[a];
				}
			}
			break;
		default:
			break;
		}
	}
	fclose( f );
}

}; // namespace KdTreeDemo
