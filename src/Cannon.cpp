#include "Cannon.h"

Cannon::Cannon(HGE *aHGE, Point aPosition, int aRoute, hgeRect aBorder, std::vector<std::shared_ptr<Fireball>> *aFireballsVector)
{
    mHge = aHGE;
    mPosition=aPosition;
    mRoute = aRoute;
    mBorder=aBorder;
    mFireballsVector = aFireballsVector;
    mCannonTexture=mHge->Texture_Load("cannon.png");
    mCannon=new hgeSprite(mCannonTexture, 0, 0, CANNON_TEXTURE_SIZE, CANNON_TEXTURE_SIZE);
    mCannon->SetHotSpot(CANNON_TEXTURE_SIZE/2, CANNON_TEXTURE_SIZE/2);
    mCannonRect = new hgeRect(mPosition.x - CANNON_TEXTURE_SIZE/2,
                              mPosition.y - CANNON_TEXTURE_SIZE/2,
                              mPosition.x + CANNON_TEXTURE_SIZE/2,
                              mPosition.y + CANNON_TEXTURE_SIZE/2);
    mTimer=0;
}

Cannon::~Cannon()
{
    mHge->Texture_Free(mCannonTexture);
    delete mCannon;
    delete mCannonRect;
}

void Cannon::Render()
{
    switch (mRoute)
    {
        case Fireball::UP:
            mCannon->RenderEx(mPosition.x, mPosition.y, 3*M_PI/2);
            break;
        case Fireball::DOWN:
            mCannon->RenderEx(mPosition.x, mPosition.y, M_PI/2);
            break;
        case Fireball::LEFT:
            mCannon->RenderEx(mPosition.x, mPosition.y, M_PI);
            break;
        case Fireball::RIGHT:
            mCannon->RenderEx(mPosition.x, mPosition.y, 0);
            break;
    }
}

void Cannon::Frame()
{
    mTimer += mHge->Timer_GetDelta();
    if (mTimer >= ATTACK_SPEED)
    {
        mTimer=0;
        switch(mRoute) {
        case Fireball::UP:
            mFireballsVector->push_back(std::shared_ptr<Fireball>(
                new Fireball(mHge, Point(mPosition.x, mPosition.y - CANNON_TEXTURE_SIZE/2), mRoute, mBorder)));
            break;
        case Fireball::DOWN:
            mFireballsVector->push_back(std::shared_ptr<Fireball>(
                new Fireball(mHge, Point(mPosition.x, mPosition.y + CANNON_TEXTURE_SIZE/2), mRoute, mBorder)));
            break;
        case Fireball::LEFT:
            mFireballsVector->push_back(std::shared_ptr<Fireball>(
                new Fireball(mHge, Point(mPosition.x - CANNON_TEXTURE_SIZE/2, mPosition.y), mRoute, mBorder)));
            break;
        case Fireball::RIGHT:
            mFireballsVector->push_back(std::shared_ptr<Fireball>(
                new Fireball(mHge, Point(mPosition.x + CANNON_TEXTURE_SIZE/2, mPosition.y), mRoute, mBorder)));
            break;
        }
    }
}
