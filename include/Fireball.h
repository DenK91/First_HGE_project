#ifndef FIREBALL_H
#define FIREBALL_H

#include <hge.h>
#include <hgesprite.h>
#include <hgeparticle.h>
#include "Utils.h"
#include <stdio.h>

#define SPEED 90
#define FIREBALL_TEXTURE_SIZE 24

class Fireball
{
    public:
        Fireball(HGE *aHGE, Point aStartPosition, int aRoute, hgeRect aBorder);
        ~Fireball();
        void Render();
        void Frame();
        Point    GetCurrentPosition();
        hgeRect* GetFireballRect();
        bool IsOutOfBorder();
        enum {UP, DOWN, LEFT, RIGHT};
    protected:
    private:
        HGE*   mHge;
        Point   mPosition;
        int     mRoute;
        hgeRect mBorder;
        hgeSprite* mFireball;
        HTEXTURE   mFireballTexture;
        hgeRect*   mFireballRect;

};

#endif // FIREBALL_H
