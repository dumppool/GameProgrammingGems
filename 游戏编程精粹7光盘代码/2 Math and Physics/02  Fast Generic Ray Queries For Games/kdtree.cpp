// -----------------------------------------------------------
// KdTreeDemo - kdtree.cpp
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#include "template.h"
#include "scene.h"
#include "memory.h"
#include "kdtree.h"
#include "surface.h"

namespace KdTreeDemo {

vector3* PrimData::s_TVert = new vector3[16];
static EBoxList* elist;
static PrimData pdat( (Primitive*)0 );

// -----------------------------------------------------------
// KdTree class implementation
// -----------------------------------------------------------
KdTree::KdTree()
{
	m_Root = MManager::NewKdTreeNodePair();
	m_ObjList = 0;
	m_CurObj = 0;
	m_TravCost = 1;
	m_IntrCost = 3;
	m_MaxDepth = 20;
	m_PPerLeaf = 2;
	m_UseFlat = true;
}

void KdTree::InitBuild( Primitive** a_Prim, unsigned int a_PCount )
{
	int prims = 0;
	ObjectList* olist = 0, *tail = 0;
	for ( unsigned int pr = 0; pr < a_PCount; pr++ )
	{
		const Primitive* prim = a_Prim[pr];
		ObjectList* newnode = MManager::NewObjectList();
		newnode->SetPrimitive( prim );
		newnode->SetNext( 0 );
		if (!tail) { tail = newnode; olist = tail; }
			  else { tail->SetNext( newnode ); tail = newnode; }
		prims++;
	}
	aabb sbox = Scene::GetExtends();
	const vector3 b1 = sbox.GetPos();
	const vector3 b2 = sbox.GetPos() + sbox.GetSize();
	BuildTree( olist, m_Root, sbox, 0, prims );
}

void KdTree::Build( Primitive** a_Prim, unsigned int a_PCount )
{
	m_CurObj = 0;
	InitBuild( a_Prim, a_PCount );
	if (!m_ObjList)
	{
		int treq = CountLeafPrims( m_Root );
		treq += (treq >> 2) | 256;
		m_ObjList = new Primitive*[treq];
	}
	BuildObjectList( m_Root, 0 );
}

int KdTree::CountLeafPrims( KdTreeNode* a_Node )
{
	int retval = 0;
	if (a_Node->IsLeaf())
	{
		ObjectList* l = a_Node->GetList();
		while (l) { retval++; l = l->GetNext(); }
	}
	else
	{
		const KdTreeNode* left = a_Node->GetLeft(), *right = a_Node->GetRight();
		if (a_Node->GetLeft()) retval += CountLeafPrims( (KdTreeNode*)left );
		if (a_Node->GetRight()) retval += CountLeafPrims( (KdTreeNode*)right );
	}
	return retval;	
}

void KdTree::BuildObjectList( KdTreeNode* a_Node, int a_Depth )
{
	if (a_Node->IsLeaf())
	{
		// convert primitives in leaves to TriAccels
		int first = m_CurObj, count = 0;
		ObjectList* l = a_Node->GetList();
		while (l)
		{
			m_ObjList[m_CurObj++] = (Primitive*)l->GetPrimitive();
			count++;
			l = l->GetNext();
		}
		if (count > 31) count = 31;
		a_Node->SetObjList( first, count );
	}
	else 
	{
		KdTreeNode* left = (KdTreeNode*)a_Node->GetLeft();
		KdTreeNode* right = (KdTreeNode*)a_Node->GetRight();
		if (left) BuildObjectList( left, a_Depth + 1 );
		if (right) BuildObjectList( right, a_Depth + 1 );
	}
}

void KdTree::BuildTree( ObjectList* a_List, KdTreeNode* a_Node, aabb& a_Box, int a_Depth, int a_Prims )
{
	elist = MManager::NewEBoxList();
	elist->init();
	ObjectList* ol = a_List;
	int events = 0;
	float pos[2];
	EBox* box, *next = MManager::NewEBox(), *first = next;
	while (ol)
	{
		const Primitive* prim = ol->GetPrimitive();
		pdat.Init( prim );
		box = next;
		next = MManager::NewEBox(); 
		box->prim = (Primitive*)prim;
		box->flags = EBox::STRADDLING;
		for ( int a = 0; a < 3; a++ )
		{
			pos[0] = pdat.bbox.m_P1.cell[a], pos[1] = pdat.bbox.m_P2.cell[a];
			box->side[0].pos[a] = pos[0];
			box->side[0].next( a, &box->side[1] );
			box->side[0].side( a, EBox::START );
			box->side[1].pos[a] = pos[1];
			box->side[1].next( a, &next->side[0] );
			box->side[1].side( a, EBox::END );
			if (pos[0] == pos[1]) 
			{
				box->side[0].side( a, EBox::PLANAR );
				box->side[1].side( a, EBox::PLANAR );
				box->side[0].next( a, box->side[1].next( a ) );
			}
		}
		ol = ol->GetNext();
	}
	int a;
	for ( a = 0; a < 3; a++ ) 
	{
		elist->head[a] = &first->side[0];
		box->side[1].next( a, 0 );
		if (box->side[1].side(a) == EBox::PLANAR) box->side[0].next( a, 0 );
		elist->sort( a );
	}
	SubdivNewONlogN( a_Node, a_Box, 0, a_Prims );
}

void KdTree::SubdivNewONlogN( KdTreeNode* a_Node, aabb& a_Box, int a_Depth, int a_Prims )
{
	// best cost calculation
	const float NA = a_Box.w() * a_Box.d() + a_Box.w() * a_Box.h() + a_Box.d() * a_Box.h();
	const float iNA = m_IntrCost / NA, minf = 0.1f + 0.01f * a_Depth;
	float bestcost = m_IntrCost * (float)a_Prims, bestpos = -1.0f;
	int i, a, bestaxis = -1, bestside = -1;
	int bestTL, bestTR, bestTP;
	for ( a = 0; a < 3; a++ ) if ((a_Box.GetP2().cell[a] - a_Box.GetP1().cell[a]) > EPSILON)
	{
		const float w = 1.0f / (a_Box.GetP2().cell[a] - a_Box.GetP1().cell[a]);
		int TL = 0, TR = a_Prims, TP = 0;
		vector3 lsize = a_Box.GetP2() - a_Box.GetP1();
		vector3 rsize = lsize;
		ebox* el = elist->head[a];
		while (el)
		{
			const float pos = el->pos[a];
			int pl = 0, pr = 0;
			while ((el) && (el->pos[a] == pos) && (el->side(a) == EBox::END)) { pl++; el = el->next(a); }
			while ((el) && (el->pos[a] == pos) && (el->side(a) == EBox::PLANAR)) { TP++; el = el->next(a); }
			while ((el) && (el->pos[a] == pos) && (el->side(a) == EBox::START)) { pr++; el = el->next(a); }
			TR -= (TP + pl);
			lsize.cell[a] = pos - a_Box.GetP1().cell[a];
			rsize.cell[a] = a_Box.GetP2().cell[a] - pos;
			const float LA = lsize.x * lsize.y + lsize.x * lsize.z + lsize.y * lsize.z;
			const float RA = rsize.x * rsize.y + rsize.x * rsize.z + rsize.y * rsize.z;
			const float f1 = (pos - a_Box.GetP1().cell[a]) * w;
			const float f2 = (a_Box.GetP2().cell[a] - pos) * w;
			float bonus1 = 1.0f, bonus2 = 1.0f, bound = 0.1f;
			const float cost1 = bonus1 * (m_TravCost + iNA * (LA * (TL + TP) + RA * TR));
			const float cost2 = bonus2 * (m_TravCost + iNA * (LA * TL + RA * (TR + TP)));
			if (cost1 < bestcost)
			{
				bestcost = cost1, bestpos = pos;
				bestside = 0, bestaxis = a;
				bestTL = TL, bestTR = TR, bestTP = TP;
			}
			if (cost2 < bestcost)
			{
				bestcost = cost2, bestpos = pos;
				bestside = 1, bestaxis = a;
				bestTL = TL, bestTR = TR, bestTP = TP;
			}
			TL += (pr + TP);
			TP = 0;
		}
	}
	if (bestside == -1)
	{
		// store primitives in leaf
		ebox* el = elist->head[0];
		while (el)
		{
			EBox* eb = (EBox*)(((unsigned long)el) & (0xffffffff - 63));
			if (eb->flags != EBox::PROCESSED)
			{
				a_Node->Add( eb->prim );
				eb->flags = EBox::PROCESSED;
			}
			el = el->next( 0 );
		}
		return;
	}
	// best cost calculated; build child nodes
	int count[2] = { bestTL + ((bestside == 0)?bestTP:0), bestTR + ((bestside == 1)?bestTP:0) };
	aabb box[2] = { a_Box, a_Box };
	box[0].GetP2().cell[bestaxis] = bestpos;
	box[1].GetP1().cell[bestaxis] = bestpos;
	KdTreeNode* node[2];
	node[0] = (KdTreeNode*)MManager::NewKdTreeNodePair();
	node[1] = (KdTreeNode*)(node[0] + 1);
	// classify EBoxes (all set to straddling by final pass)
	ebox* el = elist->head[bestaxis];
	while (el)
	{
		const int side = el->side(bestaxis);
		EBox* eb = (EBox*)(((unsigned long)el) & (0xffffffff - 63));
		if ((side == EBox::END) && (el->pos[bestaxis] <= bestpos))
			eb->flags = EBox::LEFTLIST;
		else if ((side == EBox::START) && (el->pos[bestaxis] >= bestpos))
			eb->flags = EBox::RIGHTLIST;
		else if (side == EBox::PLANAR)
		{
			if (bestside == 0)
				eb->flags = (el->pos[bestaxis] <= bestpos)?EBox::LEFTLIST:EBox::RIGHTLIST;
			else
				eb->flags = (el->pos[bestaxis] >= bestpos)?EBox::RIGHTLIST:EBox::LEFTLIST;
		}
		el = el->next( bestaxis );
	}
	el = elist->head[bestaxis];
	// Split elist to cl[0] and cl[1]
	EBoxList cl[2];
	for ( a = 0; a < 3; a++ )
	{
		ebox* el = elist->head[a], *llp = 0, *rlp = 0;
		while (el)
		{
			ebox* next = el->next( a );
			EBox* eb = (EBox*)(((unsigned long)el) & (0xffffffff - 63));
			if (eb->flags == EBox::LEFTLIST)
			{
				if (llp) llp->next( a, el ); else { llp = el; cl[0].head[a] = el; }
				el->next( a, 0 );
				llp = el;
			}
			else if (eb->flags == EBox::RIGHTLIST)
			{
				if (rlp) rlp->next( a, el ); else { rlp = el; cl[1].head[a] = el; }
				el->next( a, 0 );
				rlp = el;
			}
			else
			{
				eb->flags = EBox::PROCESSED;
				if (!eb->clone)
				{
					eb->clone = MManager::NewEBox();
					memcpy( eb->clone, eb, 64 );
				}
				const int oside = (el->side( a ) == EBox::END)?1:0;
				ebox* clel = &eb->clone->side[oside];
				if (llp) llp->next( a, el ); else { llp = el; cl[0].head[a] = el; }
				el->next( a, 0 );
				llp = el;
				if (rlp) rlp->next( a, clel ); else { rlp = clel; cl[1].head[a] = clel; }
				clel->next( a, 0 );
				rlp = clel;
			}
			el = next;
		}
	}
	// remove invalid primitives from ll and rl
	for ( i = 0; i < 2; i++ )
	{
		el = cl[i].head[0];
		bool needsort = false, needclean = false;
		// clip
		while (el)
		{
			EBox* eb = (EBox*)(((unsigned long)el) & (0xffffffff - 63));
			if (eb->flags == EBox::PROCESSED)
			{
				eb->flags = EBox::LEFTLIST + i;
				eb->clone = 0;
				pdat.Init( eb->prim );
				pdat.RebuildAndClip( box[i] );
				if (pdat.m_Verts < 3)
				{
					for ( a = 0; a < 3; a++ )
					{
						eb->side[0].side( a, EBox::INVALID );
						eb->side[1].side( a, EBox::INVALID );
					}
					count[i]--;
					needclean = true;
				}
				else 
				{
					for ( a = 0; a < 3; a++ )
					{
						eb->side[0].pos[a] = pdat.bbox.m_P1.cell[a];
						eb->side[1].pos[a] = pdat.bbox.m_P2.cell[a];
					}
					needsort = true;
				}
			}
			eb->flags = EBox::STRADDLING;
			el = el->next( 0 );
		}
		// clean and resort
		for ( a = 0; a < 3; a++ )
		{
			if (needclean)
			{
				el = cl[i].head[a];
				ebox* prev = 0;
				while (el)
				{
					if (el->side( a ) == EBox::INVALID)
					{
						if (!prev) cl[i].head[a] = el->next( a );
							  else prev->next( a, el->next( a ) );
					}
					else prev = el;
					el = el->next( a );
				}
			}
			if ((needsort) && (cl[i].head[a])) cl[i].sort( a );
		}
	}
	// recurse
	if (a_Depth < m_MaxDepth)
	{
		a_Node->SetSplitPos( bestpos );
		a_Node->SetAxis( bestaxis );
		a_Node->SetLeft( node[0] );
		a_Node->SetLeaf( false );
		for ( i = 0; i < 2; i++ ) if (count[i] > m_PPerLeaf) 
		{
			elist = &cl[i];
			SubdivNewONlogN( node[i], box[i], a_Depth + 1, count[i] );
		}
		else
		{
			// store primitives in leaf
			ebox* el = cl[i].head[0];
			while (el)
			{
				EBox* eb = (EBox*)(((unsigned long)el) & (0xffffffff - 63));
				if (eb->flags != EBox::PROCESSED)
				{
					node[i]->Add( eb->prim );
					eb->flags = EBox::PROCESSED;
				}
				el = el->next( 0 );
			}
		}
	}
}

// -----------------------------------------------------------
// PrimData class implementation
// -----------------------------------------------------------
PrimData::PrimData()
{
	m_Vertex = new vector3[10];
}

PrimData::PrimData( PrimData* a_PD )
{
	m_Vertex = new vector3[10];
	m_Prim = a_PD->m_Prim;
	m_Verts = a_PD->m_Verts;
	bbox.m_P1 = a_PD->bbox.m_P1;
	bbox.m_P2 = a_PD->bbox.m_P2;
	for ( int i = 0; i < m_Verts; i++ ) m_Vertex[i] = a_PD->m_Vertex[i];
}

PrimData::PrimData( Primitive* a_Prim )
{
	m_Vertex = new vector3[10];
	Init( a_Prim );
}

void PrimData::Init( const Primitive* a_Prim )
{
	m_Prim = (Primitive*)a_Prim;
	m_Verts = 3;
	bbox.m_P1 = vector3( 999,999,999 );
	bbox.m_P2 = vector3( -999, -999, -999 );
	if (a_Prim)
	{
		for ( int i = 0; i < 3; i++ ) m_Vertex[i] = a_Prim->GetVertex( i )->GetPos();
		UpdateBBox();
	}
}

void PrimData::UpdateBBox()
{
	bbox.m_P1 = vector3( 999,999,999 );
	bbox.m_P2 = vector3( -999, -999, -999 );
	for ( int i = 0; i < m_Verts; i++ ) 
	{
		vector3 v = m_Vertex[i];
		for ( int a = 0; a < 3; a++ )
		{
			if (v.cell[a] < bbox.m_P1.cell[a]) bbox.m_P1.cell[a] = v.cell[a];
			if (v.cell[a] > bbox.m_P2.cell[a]) bbox.m_P2.cell[a] = v.cell[a];
		}
	}
}

bool PrimData::RebuildAndClip( aabb& a_Box )
{
	m_Verts = 3;
	for ( int i = 0; i < 3; i++ ) m_Vertex[i] = m_Prim->GetVertex( i )->GetPos();
	bool retval = false;
	retval |= Clip( a_Box.GetP1().x, 1, 0 );
	retval |= Clip( a_Box.GetP2().x, -1, 0 );
	retval |= Clip( a_Box.GetP1().y, 1, 1 );
	retval |= Clip( a_Box.GetP2().y, -1, 1 );
	retval |= Clip( a_Box.GetP1().z, 1, 2 );
	retval |= Clip( a_Box.GetP2().z, -1, 2 );
	if (retval) UpdateBBox();
	return retval;
}

bool PrimData::Clip( float a_Pos, float a_Dir, int a_Axis )
{
	int i, ncount = 0;
	vector3 v1 = m_Vertex[0];
	float d1 = a_Dir * (v1.cell[a_Axis] - a_Pos);
	bool allin = true, allout = true, inside = (d1 >= 0);
	for ( i = 0; i < m_Verts; i++ ) 
	{
		float dist = a_Dir * (m_Vertex[i].cell[a_Axis] - a_Pos);
		if (dist < 0) allin = false; else allout = false;
	}
	if (allin) return false;
	if (allout) { m_Verts = 0; return true; }
	for ( i = 0; i < m_Verts; i++ )
	{
		vector3 v2 = m_Vertex[(i + 1) % m_Verts];
		float d2 = a_Dir * (v2.cell[a_Axis] - a_Pos);
		if (inside && (d2 >= 0)) s_TVert[ncount++] = v2;
		else if ((!inside) && (d2 >= 0))
		{
			float d = d1 / (d1 - d2);
			vector3 vc = v1 + d * (v2 - v1);
			vc.cell[a_Axis] = a_Pos;
			s_TVert[ncount++] = vc;
			s_TVert[ncount++] = v2;
			inside = true;
		}
		else if (inside && (d2 < 0))
		{
			float d = d2 / (d2 - d1);
			vector3 vc = v2 + d * (v1 - v2);
			vc.cell[a_Axis] = a_Pos;
			s_TVert[ncount++] = vc;
			inside = false;
		}
		v1 = v2, d1 = d2;
	}
	int nout = 0;
	for ( i = 0; i < ncount; i++ ) 
	{
		vector3 dist = s_TVert[i] - s_TVert[(i + ncount - 1) % ncount];
		if (dist.Length() > 0.00001f) m_Vertex[nout++] = s_TVert[i];
	}
	m_Verts = nout;
	return true;
}

// -----------------------------------------------------------
// KdTreeNode methods
// -----------------------------------------------------------
void KdTreeNode::Add( Primitive* a_Prim )
{
	ObjectList* lnode = MManager::NewObjectList();
	lnode->SetPrimitive( a_Prim );
	lnode->SetNext( GetList() );
	SetList( lnode );
}

// -----------------------------------------------------------
// EboxList methods
// -----------------------------------------------------------
void EBoxList::sort( int a ) // mergesort, by S. Tatham
{
	ebox* p, *q, *e;
	int insize = 1, nmerges, psize, qsize, i;
	while (1) 
	{
		p = head[a]; head[a] = tail[a] = 0; nmerges = 0;  
		while (p) 
		{
			nmerges++, q = p, psize = 0;
			for ( i = 0; i < insize; i++ ) { psize++; q = q->next(a); if (!q) break; }
			qsize = insize;
			while (psize > 0 || (qsize > 0 && q)) 
			{
				if (psize == 0) { e = q; q = q->next(a); qsize--; } 
				else if (qsize == 0 || !q) { e = p; p = p->next(a); psize--; } else
				{
					float v1 = p->pos[a], v2 = q->pos[a];
					if ((v1 < v2) || ((v1 == v2) && (p->side(a) < q->side(a)))) { e = p; p = p->next(a); psize--; } 
					else { e = q; q = q->next(a); qsize--; }
				}
				if (tail[a]) tail[a]->next(a, e); else head[a] = e;
				tail[a] = e;
			}
			p = q;
		}
		tail[a]->next( a, 0 );
		if (nmerges <= 1) break;
		insize *= 2;
	}
}

void EBoxList::insert( int a, ebox* n )
{
	ebox* l = head[a], *prev = 0;
	float pos = n->pos[a];
	int side = n->side( a );
	while (l)
	{
		float lp = l->pos[a];
		int ls = l->side( a );
		if ((lp > pos) || ((lp == pos) && (ls >= side)))
		{
			if (prev) { n->next( a, prev->next( a ) ); prev->next( a, n ); }
				 else { n->next( a, head[a] ); head[a] = n; }
			return;
		}
		prev = l;
		l = l->next( a );
	}
	n->next( a, 0 );
	if (prev) prev->next( a, n ); else head[a] = n;
}

void EBoxList::remove( int a, ebox* n )
{
	ebox* l = head[a], *prev = 0;
	while (l)
	{
		if (l == n)
		{
			if (prev) prev->next( a, l->next( a ) );
				 else head[a] = l->next( a );
			return;
		}
		prev = l;
		l = l->next( a );
	}
}

}; // namespace KdTreeDemo