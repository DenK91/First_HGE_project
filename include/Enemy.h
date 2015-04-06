#ifndef ENEMY_H
#define ENEMY_H

#include <hge.h>
#include <hgesprite.h>
#include <math.h>
#include <vector>
#include "Utils.h"

#define SPEED 90
#define ENEMY_TEXTURE_SIZE 24

class Enemy
{
    public:
        Enemy(HGE* aHGE, Point aPoint);
        ~Enemy();
        void Render();
        void Frame();
        void SetTarget(Point aTarget);
        bool IsMoving();
        Point GetCurrentPosition();
        void  SetPatrolPath(std::vector<Field> aPath);
        Field GetCurrentTarget();
        Field FindNextPatrolTarget();
    protected:
    private:
        Point mTarget;
        Point mPosition;
        bool  mIsMoving;
        HGE*  mHge;
        hgeSprite* mEnemy;
        HTEXTURE   mEnemyTex;
        Field      mCurrentTarget;
        unsigned int       mCurrentPatrolTarget;
        std::vector<Field> mEnemyPatrolPath;
};

#endif // ENEMY_H
