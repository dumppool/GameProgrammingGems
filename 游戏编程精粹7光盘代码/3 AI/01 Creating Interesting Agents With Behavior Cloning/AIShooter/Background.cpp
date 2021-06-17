#include <stdlib.h>
#include <string.h>

#include "Common.h"
#include "Background.h"

Background::Background (int w, int h, float vw, float vh)
: mWidth (vw), mHeight (vh)
{
    glGenTextures (1, &mTexture);

    int size = w * h;
    
    unsigned char *pixels = new unsigned char [size];
    memset (pixels, 0, size);

    int stars = size / 1000;

    for (int i = 0; i < stars; i ++) {
        int x = rand () % w;
        int y = rand () % h;
        unsigned char color = ((unsigned char)rand () % 192) + 64;
        pixels [x + y * h] = color;
    }

    glBindTexture (GL_TEXTURE_2D, mTexture);

    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);

    delete [] pixels;
}

Background::~Background ()
{
    glDeleteTextures (1, &mTexture);
}

void Background::draw (float x, float y)
{
    glPushMatrix ();

    glPushAttrib (GL_ENABLE_BIT | GL_TRANSFORM_BIT);

    glMatrixMode (GL_TEXTURE);

    glPushMatrix ();

    glTranslatef (x, y, 0);

    glBindTexture (GL_TEXTURE_2D, mTexture);

    glEnable (GL_TEXTURE_2D);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glColor3f (1, 1, 1);
    glBegin (GL_QUADS);
    glTexCoord2f (0, 0);
    glVertex2f (-mWidth, -mHeight);
    glTexCoord2f (1, 0);
    glVertex2f (mWidth, -mHeight);
    glTexCoord2f (1, 1);
    glVertex2f (mWidth, mHeight);
    glTexCoord2f (0, 1);
    glVertex2f (-mWidth, mHeight);
    glEnd ();

    glPopMatrix ();

    glPopAttrib ();

    glPopMatrix ();
}
