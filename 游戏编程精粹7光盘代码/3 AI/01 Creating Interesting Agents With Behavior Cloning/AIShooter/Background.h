#ifndef __AISHOOTER_BACKGROUND_H__
#define __AISHOOTER_BACKGROUND_H__

/**
 * The stars background.
 * Could conceivably be any background.
 */
class Background
{
public:
    /**
     * Initialize the background.
     * \param w the width of the background texture
     * \param h the height of the background texture
     * \param vw the width of the screen, in world coordinates
     * \param vh the height of the screen, in world coordinates
     */
    Background (int w, int h, float vw, float vh);
    ~Background ();

    /**
     * Draw the background.
     * \param x the x coordinate to draw at
     * \param y the y coordinate to draw at
     */
    void draw (float x, float y);
private:
    float mWidth, mHeight;
    GLuint mTexture;
};

#endif
