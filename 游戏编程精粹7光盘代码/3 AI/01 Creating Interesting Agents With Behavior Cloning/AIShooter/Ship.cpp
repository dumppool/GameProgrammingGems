#include <assert.h>
#include <algorithm>

#include "Common.h"
#include "Bullet.h"
#include "Ship.h"

#ifdef min
#undef min
#endif

#define SPEED_CAP 0.005f
#define DIR_SPEED_CAP 2.5f

Ship::Ship (Vector2 position, float direction, float red, float green,
    float blue)
    : mRed (red), mGreen (green), mBlue (blue), mPosition (position),
    mDirection (direction), mDirectionChange (0), mAccel (ACCEL_NONE),
    mTurn (TURN_NONE), mFiring (false), mHP (100), mBullet (NULL)
{
    mDisplayList = glGenLists (1);

    glNewList (mDisplayList, GL_COMPILE);
    glColor3f (red, green, blue);
    glBegin (GL_TRIANGLES);
    glVertex2f (0, 0.05f);
    glVertex2f (-0.025f, -0.05f);
    glVertex2f (0.025f, -0.05f);
    glEnd ();
    glEndList ();
}

Ship::~Ship ()
{
    if (mDisplayList != 0) {
        glDeleteLists (mDisplayList, 1);
    }
}

void Ship::collide (GameObject* object)
{
    // Collide only with bullets at this time
    Bullet *bullet = dynamic_cast<Bullet *> (object);
    if (!bullet || bullet == mBullet) {
        return;
    }

    // Transform the vector into object space
    Vector2 rel = bullet->getPosition () - mPosition;
    rel.rotate (-mDirection);

    float x1 = 0, y1 = 0.05f;
    float x2 = -0.025f, y2 = -0.05f;
    float x3 = 0.025f, y3 = -0.05f;
    float a0 = fabs ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1));
    float a1 = fabs ((x1-rel.x)*(y2-rel.y)-(x2-rel.x)*(y1-rel.y));
    float a2 = fabs ((x2-rel.x)*(y3-rel.y)-(x3-rel.x)*(y2-rel.y));
    float a3 = fabs ((x3-rel.x)*(y1-rel.y)-(x1-rel.x)*(y3-rel.y));

    float f = fabs (a1+a2+a3-a0);
    if (f < 0.003) {
        float p = 1.0f - std::min (rel.magnitude () / 0.05f, 1.0f);
        mHP -= 5 + (int) (5.0f * p);
        bullet->die ();
    }
}

bool Ship::update ()
{
    if(mTurn == TURN_NONE) {
        mDirectionChange *= 0.95f;
    } else {
        float max = DIR_SPEED_CAP;
        if (mTurn == TURN_LEFT) {
            mDirectionAccel = max * 0.25f;
        } else {
            mDirectionAccel = max * -0.25f;
        }

        mDirectionChange += mDirectionAccel;
        if (fabs (mDirectionChange) > max) {
            float ratio = max / fabs (mDirectionChange);
            mDirectionChange *= ratio;
        }
    }

    if (mAccel == ACCEL_NONE) {
        mVelocity *= 0.98f;
    } else {
        float max = SPEED_CAP;
        if (mAccel == ACCEL_REVERSE) {
            max *= -0.5f;
        }

        float accel = max * 0.05f;

        mAcceleration = Vector2 (0, accel);
        mAcceleration.rotate (mDirection);

        mVelocity += mAcceleration;
        if (mVelocity.magnitudeSquared () > (max * max)) {
            float ratio = fabs (max) / mVelocity.magnitude ();
            mVelocity *= ratio;
        }
    }

    if (mFiring && !mBullet) {
        mBullet = new Bullet (this, mPosition, mDirection, mRed, mGreen, mBlue);
        AIShooter::getInstance ()->addObject (mBullet);
    }

    mPosition += mVelocity;
    mDirection += mDirectionChange;
    while (mDirection < -180.0f) {
        mDirection += 360.0f;
    }
    while (mDirection > 180.0f) {
        mDirection -= 360.0f;
    }

    if (mHP <= 0) {
        return false;
    }

    return true;
}

void Ship::draw ()
{
    glPushMatrix ();

    glTranslatef (mPosition.x, mPosition.y, 0);

    float green = ((float) mHP / 100.0f) * 0.1f;

    glBegin (GL_QUADS);
    glColor3f (0, 0.75f, 0);
    glVertex2f (-0.05f, 0.07f);
    glVertex2f (-0.05f, 0.06f);
    glVertex2f (-0.05f + green, 0.06f);
    glVertex2f (-0.05f + green, 0.07f);
    if (mHP < 100) {
        glColor3f (0.75f, 0, 0);
        glVertex2f (-0.05f + green, 0.07f);
        glVertex2f (-0.05f + green, 0.06f);
        glVertex2f (0.05f, 0.06f);
        glVertex2f (0.05f, 0.07f);
    }
    glEnd ();

    glRotatef (mDirection, 0, 0, 1);

    glCallList (mDisplayList);

    glPopMatrix ();
}

void Ship::fire (bool firing)
{
    mFiring = firing;
}

void Ship::bulletDied (const Bullet* bullet)
{
    assert (bullet->getOwner () == this);

    mBullet = NULL;
}
