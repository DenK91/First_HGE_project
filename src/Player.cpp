#include "Player.h"

Player::Player(HGE *aHGE, Point aPoint)
{
    mHge = aHGE;
    mPosition = aPoint;
    mTarget = mPosition;
    mPlayerTex = mHge->Texture_Load("player.png");
    mPlayer = new hgeSprite(mPlayerTex, 0, 0, PLAYER_TEXTURE_SIZE, PLAYER_TEXTURE_SIZE);
    mPlayer->SetHotSpot(PLAYER_TEXTURE_SIZE / 2, PLAYER_TEXTURE_SIZE / 2);
    mIsMoving = false;
    mPlayerRect = new hgeRect(mPosition.x-PLAYER_TEXTURE_SIZE / 2,
                              mPosition.y-PLAYER_TEXTURE_SIZE / 2,
                              mPosition.x+PLAYER_TEXTURE_SIZE / 2,
                              mPosition.y+PLAYER_TEXTURE_SIZE / 2);
}

Player::~Player()
{
    mHge->Texture_Free(mPlayerTex);
    delete mPlayer;
    delete mPlayerRect;
}

hgeRect* Player::GetRect()
{
    return mPlayerRect;
}

Point Player::GetCurrentPosition()
{
    return mPosition;
}

void Player::SetTarget(Point aTarget)
{
    mTarget = aTarget;
}

bool Player::IsMoving()
{
    return mIsMoving;
}

void Player::Render()
{
    mPlayer->Render(mPosition.x, mPosition.y);
}

void Player::Frame()
{
    if (mPosition.x != mTarget.x || mPosition.y != mTarget.y) {
        float dTime = mHge->Timer_GetDelta();
        float dlina1 = sqrt(pow(mTarget.x - mPosition.x, 2) + pow(mTarget.y - mPosition.y,2));
        mPosition.x = mPosition.x + dTime * SPEED * (mTarget.x-mPosition.x) / dlina1;
        mPosition.y = mPosition.y + dTime * SPEED * (mTarget.y-mPosition.y) / dlina1;
        float dlina2 = sqrt(pow(mTarget.x - mPosition.x, 2) + pow(mTarget.y - mPosition.y,2));
        mPlayerRect->Set(mPosition.x - PLAYER_TEXTURE_SIZE / 2,
                         mPosition.y - PLAYER_TEXTURE_SIZE / 2,
                         mPosition.x + PLAYER_TEXTURE_SIZE / 2,
                         mPosition.y + PLAYER_TEXTURE_SIZE / 2);
        if (dlina2 >= dlina1) {
            mPosition = mTarget;
            mPlayerRect->Set(mPosition.x - PLAYER_TEXTURE_SIZE / 2,
                             mPosition.y - PLAYER_TEXTURE_SIZE / 2,
                             mPosition.x + PLAYER_TEXTURE_SIZE / 2,
                             mPosition.y + PLAYER_TEXTURE_SIZE / 2);
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
