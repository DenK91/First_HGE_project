#include "Utils.h"

Utils::Utils(HGE *hge)
{
    mHGE = hge;
    mWaitUpRightButton = false;
    mIsRightClick = false;
    mWaitUpLeftButton = false;
    mIsLeftClick = false;
}

Utils::~Utils(){}

bool Utils::IsRightButtonClick()
{
    if (mHGE->Input_GetKeyState(HGEK_RBUTTON))
    {
        if (mWaitUpRightButton)
        {
            mIsRightClick = false;
        }
        else
        {
            mIsRightClick = true;
            mWaitUpRightButton = true;
        }
    }
    else
    {
        if (mWaitUpRightButton)
        {
            mWaitUpRightButton = false;
        }
    }
    return mIsRightClick;
}

bool Utils::IsLeftButtonClick()
{
    if (mHGE->Input_GetKeyState(HGEK_LBUTTON))
    {
        if (mWaitUpLeftButton)
        {
            mIsLeftClick = false;
        }
        else
        {
            mIsLeftClick = true;
            mWaitUpLeftButton = true;
        }
    }
    else
    {
        if (mWaitUpLeftButton)
        {
            mWaitUpLeftButton = false;
        }
    }
    return mIsLeftClick;
}

