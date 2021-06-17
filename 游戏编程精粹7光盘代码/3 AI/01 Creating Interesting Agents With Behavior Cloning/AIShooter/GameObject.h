#ifndef __AISHOOTER_GAMEOBJECT_H__
#define __AISHOOTER_GAMEOBJECT_H__

#include "Vector2.h"

/**
 * Common interface for all game objects.
 */
class GameObject
{
public:
    virtual ~GameObject () { }

    /**
     * Update the game object.
     * \return \c false if the object should be destroyed
     */
    virtual bool update () = 0;

    /**
     * Draw the object.
     */
    virtual void draw () = 0;

    /**
     * \return the object's current direction, in degrees
     */
    virtual float getDirection () const = 0;

    /**
     * \return the object's current position vector
     */
    virtual const Vector2& getPosition () const = 0;

    /**
     * Check collision with another object (and react accordingly).
     * \param object the object to collide against
     */
    virtual void collide (GameObject* object) = 0;
};

#endif
