#include "Fireball.h"

Fireball::Fireball(HGE *aHGE, Point aStartPosition, int aRoute, hgeRect aBorder)
{
    mHge = aHGE;
    mRoute = aRoute;
    mPosition = aStartPosition;
    mBorder = aBorder;
    mFireballTexture = mHge->Texture_Load("fireball.png");
    mFireball = new hgeSprite(mFireballTexture, 0, 0, FIREBALL_TEXTURE_SIZE, FIREBALL_TEXTURE_SIZE);
    mFireball->SetColor(0xFFFFA000);
    mFireball->SetHotSpot(FIREBALL_TEXTURE_SIZE/2,FIREBALL_TEXTURE_SIZE/2);
    mFireballRect = new hgeRect(mPosition.x-FIREBALL_TEXTURE_SIZE/2,
                                mPosition.y-FIREBALL_TEXTURE_SIZE/2,
                                mPosition.x+FIREBALL_TEXTURE_SIZE/2,
                                mPosition.y+FIREBALL_TEXTURE_SIZE/2);
}

Fireball::~Fireball()
{
    mHge->Texture_Free(mFireballTexture);
    delete mFireball;
    delete mFireballRect;
}

Point Fireball::GetCurrentPosition()
{
    return mPosition;
}

hgeRect* Fireball::GetFireballRect()
{
    return mFireballRect;
}

bool Fireball::IsOutOfBorder()
{
    if (mPosition.x >= mBorder.x1 && mPosition.x <= mBorder.x2 &&
        mPosition.y >= mBorder.y1 && mPosition.y <= mBorder.y2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Fireball::Render()
{
    mFireball->Render(mPosition.x, mPosition.y);
}

void Fireball::Frame()
{
    float dx=0.0f, dy=0.0f;
    float dt=mHge->Timer_GetDelta();
	if (mRoute == LEFT)  dx -= SPEED * dt;
	if (mRoute == RIGHT) dx += SPEED * dt;
	if (mRoute == UP)    dy -= SPEED * dt;
	if (mRoute == DOWN)  dy += SPEED * dt;
    if (!IsOutOfBorder())
    {
        mPosition.x += dx;
        mPosition.y +=dy;
    }
    mFireballRect->Set(mPosition.x-FIREBALL_TEXTURE_SIZE/2,
                       mPosition.y-FIREBALL_TEXTURE_SIZE/2,
                       mPosition.x+FIREBALL_TEXTURE_SIZE/2,
                       mPosition.y+FIREBALL_TEXTURE_SIZE/2);
}
