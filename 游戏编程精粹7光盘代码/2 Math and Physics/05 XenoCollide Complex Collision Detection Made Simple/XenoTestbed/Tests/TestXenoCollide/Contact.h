#pragma once

#include <list>

class Contact;
class ContactConstraint;
class RigidBody;

typedef std::list<Contact*> ContactList;

class Contact
{

public:

	ContactConstraint*	constraint;
	Vector				point;
	int32				timeStamp;
	RigidBody*			b1;
	RigidBody*			b2;
	Vector				local1;
	Vector				local2;
	Vector				normal;

	Contact(RigidBody* b1, RigidBody* b2, const Vector& p1, const Vector& p2, const Vector& n);
	~Contact();

	void Update(const Vector& p1, const Vector& p2, const Vector& n);
	float32 Distance();
};

