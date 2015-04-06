#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <hge.h>
#include <hgerect.h>
#include <stdio.h>
#include <vector>
#include "Utils.h"

#define FIELD_TEXTURE_SIZE 24

class GameField {
public:
    GameField(HGE* hge, Utils* aUtils, Point aPoint, int aSize, Field aFinishField);
    ~GameField();
    void Render();
    void FrameCalculate(std::vector<Field> aCurrentOccupiedFields);
    Point GetCenter(Field aField);
    bool  IsOccupied(Field aField);
    Field GetTarget();
    Field GetFinish();
    Field FindFieldByPoint(Point aPoint);
    std::vector<Field> FindPath(Field aFieldStart, Field aFieldFinish);
    hgeRect GetBorderRect();
protected:
private:
    void FieldInit();
    void SwitchStateOnField(Field aField);
    HGE*   mHGE;
    Utils* mUtils;
    int    mSize;
    Point  mCoordTabel;
    hgeQuad**  mQuadArray;
    hgeRect*** mRectArray;
    HTEXTURE mNoSelectTexture;
    HTEXTURE mSelectTexture;
    HTEXTURE mOccupiedNoSelectTexture;
    HTEXTURE mOccupiedSelectTexture;
    HTEXTURE mFinishTexture;
    int** mFieldData;
    Field mTarget;
    Field mFinishField;
};

#endif // GAMEFIELD_H
