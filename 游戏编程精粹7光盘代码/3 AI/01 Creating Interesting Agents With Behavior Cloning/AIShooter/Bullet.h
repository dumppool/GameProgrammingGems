#ifndef __AISHOOTER_BULLET_H__
#define __AISHOOTER_BULLET_H__

#include "Ship.h"
#include "GameObject.h"

/**
 * Bullet game object.
 */
class Bullet : public GameObject
{
public:
    /**
     * Construct a bullet.
     * \param owner the ship that fired the bullet
     * \param position the initial position
     * \param direction the initial direction
     * \param red the red component of the bullet's color
     * \param green the green component of the bullet's color
     * \param blue the blue component of the bullet's color
     */
    Bullet (Ship* owner, const Vector2& position, float direction,
        float red, float green, float blue);
    virtual ~Bullet ();

    /**
     * \return the ship that fired the bullet
     */
    const Ship* getOwner () const
    {
        return mOwner;
    }

    /**
     * Tell this bullet to die (such as when it hits something)
     * \note This is hackish
     */
    void die ()
    {
        mTime = 0;
    }

    // GameObject implementation
    bool update ();
    void draw ();

    float getDirection () const
    {
        return mDirection;
    }

    const Vector2& getPosition () const
    {
        return mPosition;
    }

    void collide (GameObject* object)
    {
        // Collisions are handled by ship
    }

private:
    Ship* mOwner;

    Vector2 mPosition;
    Vector2 mVelocity;

    float mDirection;
    float mRed, mGreen, mBlue;

    int mTime;
};

#endif
