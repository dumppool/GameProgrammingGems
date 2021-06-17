#ifndef __AISHOOTER_RENDERER_H__
#define __AISHOOTER_RENDERER_H__

#include <queue>
#include <list>
#include <DataSet.h>
#include <FeatureMap.h>

class GameObject;
class Background;
class Ship;
class Agent;

/**
 * Main game class. Handles initialization, game logic and rendering of all
 * objects.
 */
class AIShooter {
    static AIShooter* sInstance;

    static void destroy ();

    /**
     * Hidden constructor
     */
    AIShooter ();
public:
    /**
     * \return the singleton instance
     */
    static AIShooter* getInstance ()
    {
        if (!sInstance) {
            sInstance = new AIShooter ();
        }
        return sInstance;
    }

    ~AIShooter ();

    /**
     * Init the game.
     * \param argc the number of arguments passed to the program
     * \param argv the vector of argument strings
     */
    void init (int argc, char **argv);

    /**
     * Run the game.
     */
    void run ();

    /**
     * Add an object to the render and update lists.
     * \param object the object to add
     */
    void addObject(GameObject* object);

private:
    // GLUT callbacks
    static void reshapeCallback (int w, int h);
    static void updateCallback (int);
    static void renderCallback ();
    static void keyCallback (unsigned char key, int x, int y);
    static void keyUpCallback (unsigned char key, int x, int y);
    static void specialKeyCallback (int key, int x, int y);
    static void specialKeyUpCallback (int key, int x, int y);

    /**
     * Update the game logic.
     */
    void update ();

    /**
     * Record the game state for later training and for the agent's use.
     */
    void recordState ();

    /**
     * Render the graphics.
     */
    void render ();

    /**
     * Save the recorded game state data for later learning.
     */
    void saveData ();

    Background *mBackground;

    std::list<GameObject*> mGameObjects;
    Ship *mPlayerShip;
    Ship *mOpponentShip;
    Agent *mOpponent;

    // Game state
    std::queue<DataSet::raw_row_t> mStateQueue;
    float mLastDistance;
    float mLastDirTo;
    float mLastDirFrom;

    // Training state
    FeatureMap mFeatureMap;
    DataSet mTrainingData;
};

#endif
