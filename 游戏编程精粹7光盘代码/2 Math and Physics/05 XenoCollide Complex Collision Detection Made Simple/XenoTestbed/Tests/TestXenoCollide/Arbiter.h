
#pragma once

#include <list>
#include <map>

#include "Contact.h"

class RigidBody;

//////////////////////////////////////////////////////////////////////////////
// Each Arbiter maintains a list of contacts between two RigidBody objects.
//
// As new points of contact between the pair are discovered, they are added
// to the Arbiter with a call to AddContact().
//
class Arbiter
{

public:

	Arbiter(RigidBody* in1, RigidBody* in2);
	~Arbiter();

	void AddContact(const Vector& p1, const Vector& p2, const Vector& normal);

public:

	RigidBody*	b1;
	RigidBody*	b2;
	ContactList	contacts;

};

//////////////////////////////////////////////////////////////////////////////
// For easy access, Arbiters are stored in a map.  To find the Arbiter for
// two RigidBodies, build an ArbiterKey for the two bodies and use it
// as the lookup key for the ArbiterMap.
//
class ArbiterKey
{

public:

	ArbiterKey(RigidBody* b1, RigidBody* b2)
	{
		if ( b1 < b2 )
		{
			this->b1 = b1;
			this->b2 = b2;
		}
		else
		{
			this->b1 = b2;
			this->b2 = b1;
		}
	}

	bool operator < (const ArbiterKey& other) const
	{
		return (b2 < other.b2) || ((b2 == other.b2) && (b1 < other.b1));
	}

private:

	RigidBody*	b1;
	RigidBody*	b2;

};

typedef std::map<ArbiterKey, Arbiter*>::iterator ArbiterIt;
