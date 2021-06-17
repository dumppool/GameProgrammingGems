// -----------------------------------------------------------
// KdTreeDemo - game.cpp
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#include "template.h"
#include "game.h"
#include "scene.h"
#include "kdtree.h"
#include "memory.h"
#include "surface.h"

using namespace KdTreeDemo;

Scene scene;
MManager memman;

__declspec(align(32))
struct KDStack
{
	KdTreeNode* node;
	float tfar, tnear;
};

static KDStack stack[60];

// -----------------------------------------------------------
// Game::TraceRay
// Trace a single ray
// Parameters:
// vector3& O    Origin of the ray
// vector3& D    Normalized direction of the ray
// float& dist   Distance of last hitpoint found
// KdTree* tree  kD-tree to use for this query
// Returns:
// The distance along the ray where the closest geometry was
// found. Hitpoint then is O + dist * D.
// -----------------------------------------------------------
void Game::TraceRay( vector3& O, vector3& D, float& dist, KdTree* tree )
{
	// prepare data for traversal
	KdTreeNode* node = tree->GetRoot();
	vector3 R( 1 / D.x, 1 / D.y, 1 / D.z );
	int oct = ((D.x < 0)?1:0) + ((D.y < 0)?2:0) + ((D.z < 0)?4:0);
	int* rdir = &m_RayDir[oct][0][0];
	int stackptr = 0;
	Primitive** objlist = tree->GetObjectList();

	// clip ray against scene extends
	aabb e = Scene::GetExtends();
	const float e1[6] = { e.GetP1().x, e.GetP2().x, e.GetP1().y, e.GetP2().y, e.GetP1().z, e.GetP2().z };
	float tnear = 0, tfar = dist;
	for ( unsigned int a = 0; a < 3; a++ )
	{
		float v1 = (e1[a * 2 + rdir[a << 1]] - O.cell[a]) * R.cell[a];
		float v2 = (e1[a * 2 + rdir[(a << 1) + 1]] - O.cell[a]) * R.cell[a];
		if (v1 > tnear) tnear = v1;
		if (v2 < tfar) tfar = dist = v2;
	}
	if (tfar <= tnear) return; // can't do, ray doesn't hit scene box

	// actual traversal
	while (1)
	{
		while (!node->IsLeaf())
		{
			int axis = node->GetAxis();
			KdTreeNode* front = node->GetLeft() + rdir[axis * 2];
			KdTreeNode* back  = node->GetLeft() + rdir[axis * 2 + 1];
			float tsplit = (node->m_Split - O.cell[axis]) * R.cell[axis];
			node = back;
			if (tsplit < tnear) continue;
			node = front;
			if (tsplit > tfar) continue;
			stack[stackptr].tfar = tfar;
			stack[stackptr++].node = back;
			tfar = MIN( tfar, tsplit );
		}

		// leaf node found, process triangles
		int start = node->GetObjOffset(), count = node->GetObjCount();
		for (int i = 0; i < count; i++ ) 
		{
			// intersect triangles
			Primitive* pr = objlist[start++];
			vector3 ao = pr->GetVertex( 0 )->m_Pos - O;
			if (DOT( ao, pr->GetNormal()) < 0) continue;
			vector3 bo = pr->GetVertex( 1 )->m_Pos - O;
			vector3 co = pr->GetVertex( 2 )->m_Pos - O;
			vector3 v1c = bo^ao, v2c = ao^co, v0c = co^bo;
			float nominator = DOT( ao, pr->GetNormal() );
			// intersection test
			float v0d = DOT( v0c, D ), v1d = DOT( v1c, D ), v2d = DOT( v2c, D );
			if ((v0d > 0) && (v1d > 0) && (v2d > 0))
			{
				float t = nominator * (1.0f / DOT( D, pr->GetNormal() ));
				if ((t < dist) && (t >= 0)) dist = t;
			}
		}

		// terminate, or pop node from stack
		if ((dist < tfar) || (!stackptr)) break;
		node = stack[--stackptr].node;
		tnear = tfar;
		tfar = stack[stackptr].tfar;
	}
}

// -----------------------------------------------------------
// Game::Init
// Initialization
// -----------------------------------------------------------
void Game::Init()
{
	vector3 pos( 0, 0, 0 );
	int i;
	char t[128];
	
	switch (m_State)
	{
	case 0:
		// let the user know what's going on
		m_Surface->Clear( 0 );
		m_Surface->Print( "building kd-tree, please wait", 10, 10, 0xffffff );
		m_State = 1;
		break;
		
	case 1:
		// load a test scene (static)
		Scene::LoadOBJ( "testdata/gemsscene.obj", pos, 0.82f, true );
		
		// load an object (dynamic)
		Scene::m_Flip = true;
		Scene::LoadOBJ( "testdata/box.obj", pos, 0.4f, false );
		
		// construct the kd-tree
		Scene::GetKdTree()->Build( Scene::GetPrimArray(), Scene::GetNrPrimitives() );
		MManager::PrepForDynamics();
		
		// done for this frame
		m_State = 2;
		break;
		
	case 2:
		// store static mesh image in a surface for fast redraw
		m_State = 3;
		m_Backdrop = new Surface( SCRWIDTH, SCRHEIGHT );
		m_Surface->Clear( 0 );
		m_Surface->Print( "scene: gemsscene.obj", 10, 10, 0xffffff );
		sprintf( t, "prims: %i", Scene::GetNrPrimitives() );
		m_Surface->Print( t, 10, 22, 0xffffff );
		DrawScene( Scene::GetPrimArray(), Scene::GetNrPrimitives(), 0x999999 );
		m_Surface->CopyTo( m_Backdrop, 0, 0 );		
		break;
		
	case 3:
		// precomputed data for ray traversal
		for ( i = 0; i < 8; i++ )
		{
			int rdx = i & 1;
			int rdy = (i >> 1) & 1;
			int rdz = (i >> 2) & 1;
			m_RayDir[i][0][0] = rdx, m_RayDir[i][0][1] = rdx ^ 1;
			m_RayDir[i][1][0] = rdy, m_RayDir[i][1][1] = rdy ^ 1;
			m_RayDir[i][2][0] = rdz, m_RayDir[i][2][1] = rdz ^ 1;
		}
		
		// setup ray rotation
		m_Angle = PI * 1.2f;
		
		// initialization done
		m_State = 999;
		break;
	}
}

// -----------------------------------------------------------
// Game::UpdateDynamicObject
// Rotate the dynamic object
// -----------------------------------------------------------
void Game::UpdateDynamicObject()
{
	// setup transformation matrix
	Primitive** dprim = Scene::GetDPrimArray();
	matrix mat;
	mat.RotateY( m_Angle / PI * 180 );
	mat.Translate( vector3( 15, -10, 12 ) );
	
	// transform all vertices of all primitives (inefficient, no dupe check)
	for ( unsigned int i = 0; i < Scene::GetNrDynamicPrims(); i++ )
	{
		Primitive* p = dprim[i];
		for ( int j = 0; j < 3; j++ ) p->GetVertex( j )->Transform( mat );
		vector3 e1 = p->GetVertex( 1 )->GetPos() - p->GetVertex( 0 )->GetPos();
		vector3 e2 = p->GetVertex( 2 )->GetPos() - p->GetVertex( 0 )->GetPos();
		vector3 N = e2.Cross( e1 );
		N.Normalize();
		p->SetNormal( N );
	}
}

// -----------------------------------------------------------
// Game::DrawScene
// Visualize an array of primitives
// -----------------------------------------------------------
void Game::DrawScene( Primitive** a_Prim, int a_Count, unsigned int a_Color )
{
	for ( int i = 0; i < a_Count; i++ )
	{
		const Primitive* p = a_Prim[i];
		const Vertex* v[3] = { p->GetVertex( 0 ), p->GetVertex( 1 ), p->GetVertex( 2 ) };
		float sx[3], sy[3];
		for ( int j = 0; j < 3; j++ )
		{	
			// transform to screen space
			sx[j] = (v[j]->GetPos().x * 0.009f) * SCRWIDTH + SCRWIDTH * 0.5f;
			sy[j] = (v[j]->GetPos().z * 0.009f) * SCRWIDTH + SCRHEIGHT * 0.5f;
		}
		m_Surface->Line( sx[0], sy[0], sx[1], sy[1], a_Color );
		m_Surface->Line( sx[1], sy[1], sx[2], sy[2], a_Color );
		m_Surface->Line( sx[2], sy[2], sx[0], sy[0], a_Color );
	}
}

// -----------------------------------------------------------
// Game::DrawRay
// Visualize a ray segment
// -----------------------------------------------------------
void Game::DrawRay( vector3& a_P1, vector3& a_P2 )
{
	// transform to screen space
	float sx1 = (a_P1.x * 0.009f) * SCRWIDTH + SCRWIDTH * 0.5f;
	float sy1 = (a_P1.z * 0.009f) * SCRWIDTH + SCRHEIGHT * 0.5f;
	float sx2 = (a_P2.x * 0.009f) * SCRWIDTH + SCRWIDTH * 0.5f;
	float sy2 = (a_P2.z * 0.009f) * SCRWIDTH + SCRHEIGHT * 0.5f;
	m_Surface->Line( sx1, sy1, sx2, sy2, 0xff0000 );
}

// -----------------------------------------------------------
// Game::Tick
// Game main loop
// -----------------------------------------------------------
void Game::Tick()
{
	// initialization
	if (m_State < 999) 
	{
		Init();
		return;
	}
	
	// render backdrop (wireframe model of static scenery)
	m_Backdrop->CopyTo( m_Surface, 0, 0 );
	
	// update the dynamic object
	UpdateDynamicObject();
	DrawScene( Scene::GetDPrimArray(), Scene::GetNrDynamicPrims(), 0x88ff88 );
	
	// prepare memory manager for rebuilding a kd-tree
	MManager::PrepRebuild();
	
	// build the kd-tree for the dynamic object
	Scene::GetDKdTree()->Build( Scene::GetDPrimArray(), Scene::GetNrDynamicPrims() );

	// trace 50 rays	
	for ( int i = 0; i < 5000; i++ )
	{
		vector3 o( 1, -10, 0 );
		vector3 t( 100 * cosf( m_Angle + i * 0.0002f ), -10, 100 * sinf( m_Angle + i * 0.0002f ) );
		vector3 d = t - o;
		d.Normalize();
		float dist = 1000;
		TraceRay( o, d, dist, Scene::GetKdTree() );		// static scenery
		TraceRay( o, d, dist, Scene::GetDKdTree() );	// dynamic scenery
		if (!(i & 63)) DrawRay( o, o + d * dist );
	}
	m_Angle += 0.02f;
	if (m_Angle > (2 * PI)) m_Angle -= 2 * PI;
}