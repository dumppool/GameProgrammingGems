#ifndef __AISHOOTER_SHIP_H__
#define __AISHOOTER_SHIP_H__

#include "AIShooter.h"
#include "GameObject.h"

class Bullet;

/**
 * Ship game object.
 */
class Ship : public GameObject
{
public:
    enum AccelType
    {
        ACCEL_NONE = 0, 
        ACCEL_NORMAL = 1,
        ACCEL_REVERSE = 2
    };

    enum TurnType
    {
        TURN_NONE = 0, 
        TURN_LEFT = 1,
        TURN_RIGHT = 2
    };

    /**
     * Construct a ship.
     * \param position the initial position
     * \param direction the initial direction
     * \param red the red component of the ship's color
     * \param green the green component of the ship's color
     * \param blue the blue component of the ship's color
     */
    Ship (Vector2 position, float direction, float red, float green,
        float blue);
    virtual ~Ship ();

    /**
     * \return this ship's current hit points
     */
    int getHitPoints () const
    {
        return mHP;
    }

    /**
     * \return this ship's current acceleration
     */
    AccelType getAccel () const
    {
        return mAccel;
    }

    /**
     * \return this ship's current turning
     */
    TurnType getTurn () const
    {
        return mTurn;
    }

    /**
     * \return this ship's current firing state
     */
    bool isFiring () const
    {
        return mFiring;
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

    void collide (GameObject* object);

    // Controls

    /**
     * Set the current acceleration mode
     * \param type the type of acceleration
     * \note You must call with type "none" to stop accelerating
     */
    void accel (AccelType type)
    {
        mAccel = type;
    }

    /**
     * Set the current turning mode
     * \param type the type of turning
     * \note You must call with type "none" to stop turning
     */
    void turn (TurnType type)
    {
        mTurn = type;
    }

    /**
     * Fire (or stop firing)
     * \param firing set to \c true if the ship is firing
     * \note Set to \c false to stop firing
     */
    void fire (bool firing);

    // Lets bullets notify owner
    void bulletDied (const Bullet* bullet);

private:
    GLuint mDisplayList;
    float mRed, mGreen, mBlue;

    Vector2 mPosition;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    float mDirection;
    float mDirectionChange;
    float mDirectionAccel;

    AccelType mAccel;
    TurnType mTurn;
    bool mFiring;

    int mHP;
    Bullet* mBullet;
};

#endif
