
#include "StdAfx.h"

#include "Arbiter.h"
#include "Contact.h"

using namespace std;

//////////////////////////////////////////////////////////////

Arbiter::Arbiter(RigidBody* in1, RigidBody* in2)
: b1(in1)
, b2(in2)
{
}

Arbiter::~Arbiter()
{
	ContactList::iterator it = contacts.begin();
	while (it != contacts.end())
	{
		delete *it;
		it++;
	}
	contacts.clear();
}

void Arbiter::AddContact(const Vector& p1, const Vector& p2, const Vector& normal)
{
	Vector point = 0.5f * (p1 + p2);

	Contact* contact = NULL;
	ContactList::iterator it = contacts.begin();
	ContactList::iterator endIt = contacts.end();
	while (it != endIt)
	{
		if ( ((*it)->point - point).Len3Squared() < 1.0f )
		{
			contact = *it;
			break;
		}
		it++;
	}

	if ( contact == NULL )
	{
		contact = new Contact(b1, b2, p1, p2, normal);
		contacts.push_back(contact);
	}
	else
	{
		contact->point = point;
		contact->Update(p1, p2, normal);
	}
}

//////////////////////////////////////////////////////////////

