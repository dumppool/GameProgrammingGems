//						Tanks
//			(c) Copyright John Manslow
//					  18/04/2001

#ifndef _CTank_
#define _CTank_

class CProjectile;

class CTank
{
public:
		//The tank's position is set up in the world class so don't pass the constructor
		//anything
	CTank();
	~CTank();

		//Returns 1 is a collision has occurred between this tanka nd the specified projectile
		//in the last time step. Returns 0 otherwise.
	int nTestForProjectileTankCollision(const CProjectile * const);

		//The tank's position
	double dxPosition,dyPosition;

		//The angle of its barrrel (in polar and rectangular form)
	double dInclination;
	double dBarrelx,dBarrely;
};

#endif