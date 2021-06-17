#include "Common.h"
#include "Bullet.h"

Bullet::Bullet (Ship* owner, const Vector2& position,
    float direction, float red, float green, float blue)
: mOwner (owner), mPosition (position), mDirection (direction),
    mRed (red), mGreen (green), mBlue (blue), mTime (50)
{
    mVelocity = Vector2 (0, 0.025);
    mVelocity.rotate (direction);
}

Bullet::~Bullet ()
{
}

bool Bullet::update ()
{
    mPosition += mVelocity;

    if (--mTime <= 0) {
        mOwner->bulletDied (this);
        return false;
    }

    return true;
}

void Bullet::draw ()
{
    glPushMatrix ();

    glTranslatef (mPosition.x, mPosition.y, 0);

    glColor3f (mRed, mGreen, mBlue);
    glBegin (GL_POINTS);
    glVertex2f (0, 0);
    glEnd ();

    glPopMatrix ();
}
