// -----------------------------------------------------------
// KdTreeDemo - game.h
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#ifndef I_GAME_H
#define I_GAME_H

namespace KdTreeDemo {

class Surface;
class KdTree;
class Primitive;
class Game
{
public:
	void SetTarget( Surface* a_Surface ) { m_Surface = a_Surface; }
	void Init();
	void TraceRay( vector3& O, vector3& D, float& dist, KdTree* tree );
	void DrawScene( Primitive** a_Prim, int a_Count, unsigned int a_Color );
	void DrawRay( vector3& a_P1, vector3& a_P2 );
	void UpdateDynamicObject();
	void Tick();
private:
	Surface* m_Surface, *m_Backdrop, *m_Image;
	float m_Angle;
	int m_RayDir[8][3][2];
	int m_State;
};

}; // namespace KdTreeDemo

#endif