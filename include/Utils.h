#ifndef UTILS_H
#define UTILS_H

#include <hge.h>

struct Field
{
    Field (int r, int c) : row(r), col(c) {}
    Field () {row = 0; col = 0;}
    void operator= (const Field &aFiled) {row = aFiled.row; col = aFiled.col;}
    bool operator== (const Field &aFiled) const
    {
        if (row == aFiled.row && col == aFiled.col){return true;}
        else {return false;}
    }
    int row;
    int col;
};

struct Point
{
    Point (int aX, int aY) : x(aX), y(aY) {}
    Point () {x = 0; y = 0;}
    void operator= (const Point &aPoint) {x = aPoint.x; y = aPoint.y;}
    bool operator== (const Point &aPoint) const
    {
        if (x == aPoint.x && y == aPoint.y){return true;}
        else {return false;}
    }
    float x;
    float y;
};

class Utils
{
    public:
        Utils(HGE *hge);
        ~Utils();
        bool IsRightButtonClick();
        bool IsLeftButtonClick();
    protected:
    private:
        HGE *mHGE;
        bool mWaitUpRightButton;
        bool mIsRightClick;
        bool mWaitUpLeftButton;
        bool mIsLeftClick;
};

#endif // UTILS_H
