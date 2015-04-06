#ifndef CANNON_H
#define CANNON_H

#include <hge.h>
#include <hgesprite.h>
#include <stdio.h>
#include <vector>
#include <memory>
#include "Utils.h"
#include "Fireball.h"

#define ATTACK_SPEED 2
#define CANNON_TEXTURE_SIZE 24

class Cannon
{
    public:
        Cannon(HGE *aHGE, Point aPosition, int aRoute, hgeRect aBorder, std::vector<std::shared_ptr<Fireball>> *aFireballsVector);
        ~Cannon();
        void Render();
        void Frame();
    protected:
    private:
        HGE* mHge;
        Point   mPosition;
        int     mRoute;
        hgeRect mBorder;
        std::vector<std::shared_ptr<Fireball>>* mFireballsVector;
        float   mTimer;
        hgeSprite* mCannon;
        HTEXTURE   mCannonTexture;
        hgeRect*   mCannonRect;
};

#endif // CANNON_H
