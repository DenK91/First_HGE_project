#include "GameField.h"

GameField::GameField(HGE* hge, Utils* aUtils, Point aPoint, int aSize, Field aFinishField)
{
    mHGE = hge;
    mUtils = aUtils;
    mCoordTabel = aPoint;
    mSize = aSize;
    mFinishField = aFinishField;
    mNoSelectTexture = mHGE->Texture_Load("field_item_no_select.png");
    mSelectTexture   = mHGE->Texture_Load("field_item_select.png");
    mOccupiedNoSelectTexture = mHGE->Texture_Load("field_item_occupied_no_select.png");
    mOccupiedSelectTexture   = mHGE->Texture_Load("field_item_occupied_select.png");
    mFinishTexture   = mHGE->Texture_Load("field_item_finish.png");
    if (!mNoSelectTexture || !mSelectTexture ||
        !mOccupiedNoSelectTexture || !mOccupiedSelectTexture ||
        !mFinishTexture)
    {
        MessageBox(NULL, "Can't load Textures", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    }
    else
    {
        mFieldData = new int*[mSize];
        for (int i = 0; i < mSize; i++)
        {
            mFieldData[i] = new int[mSize];
        }
        mQuadArray = new hgeQuad*[mSize];
        for (int i = 0; i < mSize; i++)
        {
            mQuadArray[i] = new hgeQuad[mSize];
        }
        mRectArray = new hgeRect**[mSize];
        for (int i = 0; i < mSize; i++)
        {
            mRectArray[i] = new hgeRect*[mSize];
        }
    }
    mTarget = Field(-1, -1);
    FieldInit();
}

GameField::~GameField()
{
    mHGE->Texture_Free(mNoSelectTexture);
    mHGE->Texture_Free(mSelectTexture);
    mHGE->Texture_Free(mOccupiedNoSelectTexture);
    mHGE->Texture_Free(mOccupiedSelectTexture);
    mHGE->Texture_Free(mFinishTexture);
    for (int col = 0; col < mSize; col++)
    {
        for (int row = 0; row  <mSize; row++)
        {
            mHGE->Texture_Free(mQuadArray[row][col].tex);
        }
    }
    for (int i = 0; i < mSize; i++)
    {
        delete mQuadArray[i];
    }
    delete mQuadArray;
    for (int i = 0; i < mSize; i++)
    {
        delete mRectArray[i];
    }
    delete mRectArray;
    for (int i = 0; i < mSize; i++)
    {
        delete mFieldData[i];
    }
    delete mFieldData;
}

void GameField::FieldInit()
{
    for (int col = 0; col < mSize; col++)
    {
        for (int row = 0; row < mSize; row++)
        {
            mFieldData[row][col] = 0;
            mQuadArray[row][col].tex = mNoSelectTexture;
            mQuadArray[row][col].blend = BLEND_DEFAULT;
            for(int i = 0; i < 4; i++)
            {
                mQuadArray[row][col].v[i].col = 0xFFFFFFFF;
            }
            mQuadArray[row][col].v[0].tx = 0.0;
            mQuadArray[row][col].v[0].ty = 0.0;
            mQuadArray[row][col].v[1].tx = 1.0;
            mQuadArray[row][col].v[1].ty = 0.0;
            mQuadArray[row][col].v[2].tx = 1.0;
            mQuadArray[row][col].v[2].ty = 1.0;
            mQuadArray[row][col].v[3].tx = 0.0;
            mQuadArray[row][col].v[3].ty = 1.0;

            mQuadArray[row][col].v[0].x = row * FIELD_TEXTURE_SIZE + mCoordTabel.x - FIELD_TEXTURE_SIZE / 2;
            mQuadArray[row][col].v[0].y = col * FIELD_TEXTURE_SIZE + mCoordTabel.y - FIELD_TEXTURE_SIZE / 2;

            mQuadArray[row][col].v[1].x = row * FIELD_TEXTURE_SIZE + mCoordTabel.x + FIELD_TEXTURE_SIZE / 2;
            mQuadArray[row][col].v[1].y = col * FIELD_TEXTURE_SIZE + mCoordTabel.y - FIELD_TEXTURE_SIZE / 2;

            mQuadArray[row][col].v[2].x = row * FIELD_TEXTURE_SIZE + mCoordTabel.x + FIELD_TEXTURE_SIZE / 2;
            mQuadArray[row][col].v[2].y = col * FIELD_TEXTURE_SIZE + mCoordTabel.y + FIELD_TEXTURE_SIZE / 2;

            mQuadArray[row][col].v[3].x = row * FIELD_TEXTURE_SIZE + mCoordTabel.x - FIELD_TEXTURE_SIZE / 2;
            mQuadArray[row][col].v[3].y = col * FIELD_TEXTURE_SIZE + mCoordTabel.y + FIELD_TEXTURE_SIZE / 2;
            mRectArray[row][col] = new hgeRect(mQuadArray[row][col].v[0].x, mQuadArray[row][col].v[0].y,
                                               mQuadArray[row][col].v[2].x, mQuadArray[row][col].v[2].y);
        }
    }
}

hgeRect GameField::GetBorderRect()
{
    hgeRect border;
    border.Set(mRectArray[0][0]->x1, mRectArray[0][0]->y1,
               mRectArray[mSize-1][mSize-1]->x2, mRectArray[mSize-1][mSize-1]->y2);
    return border;
}

void GameField::Render()
{
    for (int col = 0; col < mSize; col++)
    {
        for (int row=0; row<mSize; row++)
        {
            mHGE->Gfx_RenderQuad(&mQuadArray[row][col]);
        }
    }
}

void GameField::FrameCalculate(std::vector<Field> aCurrentOccupiedFields)
{
    bool isBuilding  = mUtils->IsRightButtonClick();
    bool isSetTarget = mUtils->IsLeftButtonClick();
    float mouseX, mouseY;
    mHGE->Input_GetMousePos(&mouseX,&mouseY);
    for (int col = 0; col < mSize; col++)
    {
        for (int row = 0; row < mSize; row++)
        {
            Field field = Field(row, col);
            bool isFinishField = (field == mFinishField);
            bool isOccupiedField = false;
            for (unsigned int i = 0; i < aCurrentOccupiedFields.size(); i++)
            {
                isOccupiedField |= (field == aCurrentOccupiedFields[i]);
            }
            if (mRectArray[row][col]->TestPoint(mouseX, mouseY))
            {
                if (isSetTarget && !IsOccupied(field)) {
                    mTarget = field;
                }
                if (isBuilding && !isFinishField && !isOccupiedField)
                {
                    SwitchStateOnField(field);
                }
                if (mFieldData[row][col] == 0)
                {
                    mQuadArray[row][col].tex = mSelectTexture;
                }
                else
                {
                    mQuadArray[row][col].tex = mOccupiedSelectTexture;
                }
            }
            else
            {
                if (mFieldData[row][col] == 0)
                {
                    mQuadArray[row][col].tex = mNoSelectTexture;
                }
                else
                {
                    mQuadArray[row][col].tex = mOccupiedNoSelectTexture;
                }
            }
            if (isFinishField)
            {
                mQuadArray[row][col].tex = mFinishTexture;
            }
        }
    }
}

Field GameField::FindFieldByPoint(Point aPoint)
{
    for (int col = 0; col < mSize; col++)
    {
        for (int row=0; row < mSize; row++)
        {
            if (mRectArray[row][col]->TestPoint(aPoint.x, aPoint.y))
            {
                return Field(row, col);
            }
        }
    }
    return Field(-1, -1);
}

void GameField::SwitchStateOnField(Field aField)
{
    if (mFieldData[aField.row][aField.col] == 0)
    {
        mFieldData[aField.row][aField.col] = 1;
    }
    else
    {
        mFieldData[aField.row][aField.col] = 0;
    }
}

Point GameField::GetCenter(Field aField)
{
    return Point((mRectArray[aField.row][aField.col]->x2 + mRectArray[aField.row][aField.col]->x1) / 2,
                 (mRectArray[aField.row][aField.col]->y2 + mRectArray[aField.row][aField.col]->y1) / 2);
}

bool GameField::IsOccupied(Field aField)
{
    if (mFieldData[aField.row][aField.col] == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

Field GameField::GetTarget()
{
    return mTarget;
}

Field GameField::GetFinish()
{
    return mFinishField;
}

std::vector<Field> GameField::FindPath(Field aFieldStart, Field aFieldFinish)
{
    int matrix[mSize][mSize][3];
    int step;
    bool added = true, result = true;
    for (int i = 0; i < mSize; i++)
    {
        for (int j = 0; j < mSize; j++)
        {
            if(mFieldData[i][j] != 0)
            {
                matrix[i][j][0] = -2; // занято
            }
            else
            {
                matrix[i][j][0] = -1; // Мы еще нигде не были
            }
        }
    }

    matrix[aFieldFinish.row][aFieldFinish.col][0] = 0;// До финиша ноль шагов - от него будем разбегаться
    step = 0; // Изначально мы сделали ноль шагов

    // Пока вершины добаляются и мы не дошли до старта
    while (added && matrix[aFieldStart.row][aFieldStart.col][0] == -1)
    {
        added = false; // Пока что ничего не добавили
        step++;        // Увеличиваем число шагов

        // Пробегаем по всей карте
        for (int i = 0; i < mSize; i++)
        {
            for(int j = 0; j < mSize; j++)
            {
                // Если (i, j) была добавлена на предыдущем шаге
                // Пробегаем по всем четырем сторонам
                if (matrix[i][j][0] == step-1)
                {
                    int _i = i + 1;
                    int _j = j;
                    // Если не вышли за пределы карты -  обрабатываем
                    if (_i >= 0 && _j >= 0 && _i < mSize && _j < mSize)
                    {
                        // Если (_i, _j) уже добавлено или непроходимо, то не обрабатываем
                        if (matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2)
                        {
                            matrix[_i][_j][0] = step; // Добав-
                            matrix[_i][_j][1] = i; // ля-
                            matrix[_i][_j][2] = j; // ем
                            added = true; // Что-то добавили
                        }
                    }

                    _i=i-1;
                    _j=j;
                    // Если не вышли за пределы карты -  обрабатываем
                    if (_i >= 0 && _j >= 0 && _i < mSize && _j < mSize)
                    {
                        // Если (_i, _j) уже добавлено или непроходимо, то не обрабатываем
                        if (matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2)
                        {
                            matrix[_i][_j][0] = step; // Добав-
                            matrix[_i][_j][1] = i; // ля-
                            matrix[_i][_j][2] = j; // ем
                            added = true; // Что-то добавили
                        }
                    }

                    _i=i;
                    _j=j+1;
                    // Если не вышли за пределы карты -  обрабатываем
                    if (_i >= 0 && _j >= 0 && _i < mSize && _j < mSize)
                    {
                        // Если (_i, _j) уже добавлено или непроходимо, то не обрабатываем
                        if (matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2)
                        {
                            matrix[_i][_j][0] = step; // Добав-
                            matrix[_i][_j][1] = i; // ля-
                            matrix[_i][_j][2] = j; // ем
                            added = true; // Что-то добавили
                        }
                    }

                    _i=i;
                    _j=j-1;
                    // Если не вышли за пределы карты -  обрабатываем
                    if (_i >= 0 && _j >= 0 && _i < mSize && _j < mSize)
                    {
                        // Если (_i, _j) уже добавлено или непроходимо, то не обрабатываем
                        if(matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2)
                        {
                            matrix[_i][_j][0] = step; // Добав-
                            matrix[_i][_j][1] = i; // ля-
                            matrix[_i][_j][2] = j; // ем
                            added = true; // Что-то добавили
                        }
                    }
                }
            }
        }
    }

    if (matrix[aFieldStart.row][aFieldStart.col][0] == -1)
    {
        result = false; // пути не существует
    }

    std::vector<Field> resultVector;
    if (result)
    {
        int _i = aFieldStart.row, _j = aFieldStart.col;
        while (matrix[_i][_j][0] != 0)
        {
            int li = matrix[_i][_j][1];
            int lj = matrix[_i][_j][2];
            _i = li;
            _j = lj;
            resultVector.push_back(Field(_i, _j));
        }
    }
    return resultVector;
}

