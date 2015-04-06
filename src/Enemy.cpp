#include "Enemy.h"

Enemy::Enemy(HGE *aHGE, Point aPoint)
{
    mHge = aHGE;
    mPosition = aPoint;
    mTarget = mPosition;
    mEnemyTex = mHge->Texture_Load("enemy.png");
    mEnemy = new hgeSprite(mEnemyTex, 0, 0, ENEMY_TEXTURE_SIZE, ENEMY_TEXTURE_SIZE);
    mEnemy->SetHotSpot(ENEMY_TEXTURE_SIZE/2, ENEMY_TEXTURE_SIZE/2);
    mIsMoving = false;
}

Enemy::~Enemy()
{
    mHge->Texture_Free(mEnemyTex);
    delete mEnemy;
}

Point Enemy::GetCurrentPosition()
{
    return mPosition;
}

void Enemy::SetTarget(Point aTarget)
{
    mTarget = aTarget;
}

bool Enemy::IsMoving()
{
    return mIsMoving;
}

Field Enemy::GetCurrentTarget()
{
    return mCurrentTarget;
}

void Enemy::Render()
{
    mEnemy->Render(mPosition.x, mPosition.y);
}

void Enemy::Frame()
{
    if (mPosition.x != mTarget.x || mPosition.y != mTarget.y)
    {
        float dTime = mHge->Timer_GetDelta();
        float dlina1 = sqrt(pow(mTarget.x - mPosition.x, 2) + pow(mTarget.y - mPosition.y,2));
        mPosition.x = mPosition.x + dTime * SPEED * (mTarget.x - mPosition.x) / dlina1;
        mPosition.y = mPosition.y + dTime * SPEED * (mTarget.y - mPosition.y) / dlina1;
        float dlina2 = sqrt(pow(mTarget.x - mPosition.x, 2) + pow(mTarget.y - mPosition.y,2));
        if (dlina2 >= dlina1)
        {
            mPosition = mTarget;
            mIsMoving = false;
            return;
        }
        mIsMoving = true;
    }
    else
    {
        mIsMoving = false;
    }
}

void Enemy::SetPatrolPath(std::vector<Field> aPath)
{
    mEnemyPatrolPath = aPath;
    mCurrentPatrolTarget = 0;
    mCurrentTarget = mEnemyPatrolPath[mCurrentPatrolTarget];
}

Field Enemy::FindNextPatrolTarget()
{
    mCurrentPatrolTarget++;
    if (mCurrentPatrolTarget >= mEnemyPatrolPath.size())
    {
        mCurrentPatrolTarget = 0;
    }
    mCurrentTarget = mEnemyPatrolPath[mCurrentPatrolTarget];
    return mCurrentTarget;
}
