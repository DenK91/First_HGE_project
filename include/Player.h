#ifndef PLAYER_H
#define PLAYER_H

#include <hge.h>
#include <hgesprite.h>
#include <math.h>
#include "Utils.h"

#define SPEED 90
#define PLAYER_TEXTURE_SIZE 24

class Player
{
    public:
        Player(HGE *aHGE, Point aPoint);
        ~Player();
        void Render();
        void Frame();
        void SetTarget(Point aTarget);
        bool IsMoving();
        Point GetCurrentPosition();
        hgeRect* GetRect();
    protected:
    private:
        Point mTarget;
        Point mPosition;
        bool mIsMoving;
        HGE *mHge;
        hgeSprite* mPlayer;
        HTEXTURE mPlayerTex;
        hgeRect *mPlayerRect;
};

#endif // PLAYER_H
