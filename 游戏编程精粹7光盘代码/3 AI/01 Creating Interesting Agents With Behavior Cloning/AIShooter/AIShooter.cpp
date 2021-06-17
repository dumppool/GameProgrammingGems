#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "Common.h"
#include "Vector2.h"
#include "Background.h"
#include "Ship.h"
#include "Bullet.h"
#include "Agent.h"
#include "AIShooter.h"

AIShooter* AIShooter::sInstance = NULL;

void AIShooter::destroy ()
{
    delete sInstance;
    sInstance = NULL;
}

AIShooter::AIShooter ()
: mBackground (NULL), mPlayerShip (NULL), mOpponentShip (NULL),
    mOpponent (NULL), mLastDistance (-1), mLastDirTo (0), mLastDirFrom (0)
{
    mFeatureMap.defineFeature ("opp_distance", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_dir_to", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_dir_from", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_vnorm", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_ddir_to", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_ddir_from", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_hp", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_turning", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("opp_accel", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("opp_fire", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("my_hp", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("my_turning", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("my_accel", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("my_fire", FeatureMap::FT_NOMINAL);

    atexit (AIShooter::destroy);
}

AIShooter::~AIShooter ()
{
    std::list<GameObject*>::iterator iter = mGameObjects.begin (),
        end = mGameObjects.end ();
    for (;iter != end; iter ++) {
        delete *iter;
    }
    if (mBackground) {
        delete mBackground;
    }
}

void AIShooter::init (int argc, char **argv)
{
    srand (time (0));

    glutInit (&argc, argv);

    if (argc < 2 || argc > 3) {
        std::cout << "Usage:" << std::endl << std::endl;
        std::cout << "  AIShooter [-s] <agent_script>" << std::endl;
        std::cout << std::endl << std::endl;
        std::cout << "  -s\tDo not randomize starting positions/directions" << std::endl;
        exit (0);
    }

    std::string filename;
    bool randomize = true;
    if (argc == 2) {
        filename = argv[1];
    } else {
        if(!strcmp(argv[1], "-s")) {
            randomize = false;
        }
        filename = argv[2];
    }

    glutInitWindowSize (512, 512);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    glutCreateWindow ("AIShooter");

    glutReshapeFunc (AIShooter::reshapeCallback);
    glutDisplayFunc (AIShooter::renderCallback);
    glutKeyboardFunc (AIShooter::keyCallback);
    glutKeyboardUpFunc (AIShooter::keyUpCallback);
    glutSpecialFunc (AIShooter::specialKeyCallback);
    glutSpecialUpFunc (AIShooter::specialKeyUpCallback);
    glutTimerFunc (20, AIShooter::updateCallback, 0);

    glPointSize (3.0f);

    mFeatureMap.loadFromFile ("features.dat");

    Vector2 playerStart (0.4f, 0.4f);
    Vector2 opponentStart (-0.4f, -0.4f);
    float playerDir = 180;
    float opponentDir = 0;

    if (randomize) {
        // Start them near enough to each other
        for (float mag = 0.0f; mag < 0.04f || mag > 1.0f;
            mag = (playerStart - opponentStart).magnitudeSquared ())
        {
            playerStart = Vector2 ((float) (rand () % 10000) / 1000.0f,
                (float) (rand () % 10000) / 1000.0f);

            opponentStart = Vector2 ((float) (rand () % 10000) / 1000.0f,
                (float) (rand () % 10000) / 1000.0f);
        }
        playerDir = (float) (rand () % 360) - 180;
        opponentDir = (float) (rand () % 360) - 180;
    }

    mPlayerShip = new Ship (playerStart, playerDir, 0, 0, 1);
    addObject (mPlayerShip);

    mOpponentShip = new Ship (opponentStart, opponentDir, 1, 0, 0);
    addObject (mOpponentShip);

    mOpponent = new Agent (mOpponentShip);
    mOpponent->init (filename);
}

void AIShooter::run ()
{
    glutMainLoop ();
}

void AIShooter::addObject(GameObject* object)
{
    mGameObjects.push_back (object);
}

void AIShooter::reshapeCallback (int w, int h)
{
    assert (sInstance);

    float vw = 1, vh = 1;

    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (w < h) {
        GLdouble vBound = (GLdouble) h / (GLdouble) w;
        gluOrtho2D (-1, 1, -vBound, vBound);
        vh = vBound;
    } else {
        GLdouble hBound = (GLdouble) w / (GLdouble) h;
        gluOrtho2D (-hBound, hBound, -1, 1);
        vw = hBound;
    }
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    if (sInstance->mBackground) {
        delete sInstance->mBackground;
    }
    sInstance->mBackground = new Background (w, h, vw, vh);
}

void AIShooter::updateCallback (int)
{
    assert (sInstance);
    
    sInstance->update ();

    glutPostRedisplay ();

    glutTimerFunc (10, AIShooter::updateCallback, 0);
}

void AIShooter::renderCallback ()
{
    assert (sInstance);
    
    sInstance->render ();
}

void AIShooter::keyCallback (unsigned char key, int, int)
{
    assert (sInstance);

    if (key == 32) {
        sInstance->mPlayerShip->fire (true);
    }
    if (key == 27) {
        sInstance->saveData ();
        exit (0);
    }
}

void AIShooter::keyUpCallback (unsigned char key, int, int)
{
    assert (sInstance);

    if (!sInstance->mPlayerShip) {
        return;
    }

    if (key == 32) {
        sInstance->mPlayerShip->fire (false);
    }
}

void AIShooter::specialKeyCallback (int key, int, int)
{
    assert (sInstance);

    if (!sInstance->mPlayerShip) {
        return;
    }

    switch (key) {
    case GLUT_KEY_LEFT: sInstance->mPlayerShip->turn (Ship::TURN_LEFT); break;
    case GLUT_KEY_RIGHT: sInstance->mPlayerShip->turn (Ship::TURN_RIGHT); break;
    case GLUT_KEY_UP: sInstance->mPlayerShip->accel (Ship::ACCEL_NORMAL); break;
    case GLUT_KEY_DOWN: sInstance->mPlayerShip->accel (Ship::ACCEL_REVERSE); break;
    }
}

void AIShooter::specialKeyUpCallback (int key, int, int)
{
    assert (sInstance);

    if (!sInstance->mPlayerShip) {
        return;
    }

    switch (key) {
    case GLUT_KEY_LEFT: 
    case GLUT_KEY_RIGHT: sInstance->mPlayerShip->turn (Ship::TURN_NONE); break;
    case GLUT_KEY_UP: 
    case GLUT_KEY_DOWN: sInstance->mPlayerShip->accel (Ship::ACCEL_NONE); break;
    }
}

void AIShooter::update ()
{
    if (mPlayerShip && mOpponentShip) {
        recordState ();

        if (mStateQueue.size () >= 15) {
            // Delay the AI's state 150ms to simulate reaction time
            mOpponent->act (mStateQueue.front ());
            mStateQueue.pop ();
        }
    }

    std::list<GameObject*>::iterator iter = mGameObjects.begin (),
        end = mGameObjects.end ();
    while (iter != end) {
        std::list<GameObject*>::iterator iter2 = mGameObjects.begin (),
            end2 = mGameObjects.end ();
        for (;iter2 != end2; iter2 ++) {
            (*iter)->collide (*iter2);
        }

        if (!(*iter)->update ()) {
            if (*iter == mPlayerShip) {
                mPlayerShip = NULL;
            } else if (*iter == mOpponentShip) {
                mOpponentShip = NULL;
            }
            delete *iter;
            iter = mGameObjects.erase (iter);
        } else {
            iter ++;
        }
    }
}

void AIShooter::recordState ()
{
    Vector2 vec_to = mPlayerShip->getPosition ()
        - mOpponentShip->getPosition ();

    // Get the distance before we normalize
    float dist = vec_to.magnitude ();

    vec_to.normalize ();

    // Player direction vector
    Vector2 p_vec_dir (0, -1);
    p_vec_dir.rotate (mPlayerShip->getDirection ());

    // Direction from player to opponent
    float dir_to = p_vec_dir.angleBetween (vec_to);

    // opponent direction vector
    Vector2 o_vec_dir (0, -1);
    o_vec_dir.rotate (mOpponentShip->getDirection ());

    // Direction from opponent to player 
    float dir_from = o_vec_dir.angleBetween (-vec_to);

    // Record data for training
    DataSet::raw_row_t my_state;
    my_state ["opp_distance"] = dist;
    my_state ["opp_dir_to"] = dir_to;
    my_state ["opp_dir_from"] = dir_from;
    my_state ["opp_vnorm"] = dist - mLastDistance;
    my_state ["opp_ddir_to"] = dir_to - mLastDirTo;
    my_state ["opp_ddir_from"] = dir_from - mLastDirFrom;
    my_state ["opp_turning"] = mOpponentShip->getTurn ();
    my_state ["opp_accel"] = mOpponentShip->getAccel ();
    my_state ["opp_fire"] = (mOpponentShip->isFiring () ? 1 : 0);
    my_state ["opp_hp"] = mOpponentShip->getHitPoints ();
    my_state ["my_hp"] = mPlayerShip->getHitPoints ();
    my_state ["my_turning"] = mPlayerShip->getTurn ();
    my_state ["my_accel"] = mPlayerShip->getAccel ();
    my_state ["my_fire"] = (mPlayerShip->isFiring () ? 1 : 0);

    mTrainingData.addRawRow (my_state, mFeatureMap);

    // Game state for active agent
    DataSet::raw_row_t agent_state;
    agent_state ["opp_distance"] = dist;
    agent_state ["opp_dir_to"] = dir_from;
    agent_state ["opp_dir_from"] = dir_to;
    agent_state ["opp_vnorm"] = dist - mLastDistance;
    agent_state ["opp_ddir_to"] = dir_from - mLastDirFrom;
    agent_state ["opp_ddir_from"] = dir_to - mLastDirTo;
    agent_state ["opp_turning"] = mPlayerShip->getTurn ();
    agent_state ["opp_fire"] = (mPlayerShip->isFiring () ? 1 : 0);
    agent_state ["opp_accel"] = mPlayerShip->getAccel ();
    agent_state ["opp_hp"] = mPlayerShip->getHitPoints ();
    agent_state ["my_hp"] = mOpponentShip->getHitPoints ();

    mStateQueue.push (agent_state);

    mLastDistance = dist;
    mLastDirFrom = dir_from;
    mLastDirTo = dir_to;
}

void AIShooter::render ()
{
    glClear (GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    Vector2 pos;

    if (mPlayerShip) {
        pos = mPlayerShip->getPosition ();
    }

    if (mBackground) {
        mBackground->draw (pos.x, pos.y);
    }

    glTranslatef (-pos.x, -pos.y, 0);

    std::list<GameObject*>::iterator iter = mGameObjects.begin (),
        end = mGameObjects.end ();
    for (;iter != end; iter ++) {
        (*iter)->draw ();
    }

    glFlush ();

    glutSwapBuffers ();
}

void AIShooter::saveData ()
{
    time_t t = time (0);
    struct tm *tdata = localtime (&t);

    std::stringstream filename;
    filename << "training_data_" << (tdata->tm_year + 1900)
        << std::setfill('0') << std::setw (2) << (tdata->tm_mon + 1)
        << std::setw (2) << tdata->tm_mday
        << std::setw (2) << tdata->tm_hour
        << std::setw (2) << tdata->tm_min
        << std::setw (2) << tdata->tm_sec
        << ".dat";

    mTrainingData.saveToFile (filename.str ());
}

