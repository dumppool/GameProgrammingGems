
#include "StdAfx.h"

#include "Constraint.h"
#include "Contact.h"
#include "RigidBody.h"

extern int32 gTimeStamp;

Contact::Contact(RigidBody* b1, RigidBody* b2, const Vector& p1, const Vector& p2, const Vector& n)
{
	this->b1 = b1;
	this->b2 = b2;

	normal = n;

	local1 = (~(b1->body->q)).Rotate( p1 - b1->body->x );
	local2 = (~(b2->body->q)).Rotate( p2 - b2->body->x );

	constraint = new ContactConstraint(b1->body, b2->body, local1, local2, n);
	point = 0.5f * (p1 + p2);
	timeStamp = gTimeStamp;
}

Contact::~Contact()
{
	delete constraint;
	constraint = NULL;
}

void Contact::Update(const Vector& p1, const Vector& p2, const Vector& n)
{
	normal = n;

	local1 = (~(b1->body->q)).Rotate( p1 - b1->body->x );
	local2 = (~(b2->body->q)).Rotate( p2 - b2->body->x );

	constraint->Update(local1, local2, n);
	timeStamp = gTimeStamp;
}

float32 Contact::Distance()
{
	Vector p1 = b1->body->q.Rotate( local1 ) + b1->body->x;
	Vector p2 = b2->body->q.Rotate( local2 ) + b2->body->x;
	return (p1 - p2).Len3();
}

