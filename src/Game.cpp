#include "Game.h"

Game::Game(HGE* aHGE)
{
    mHge = aHGE;
    mGameUtils = new Utils(mHge);
    mStartField = Field(0, 0);
    mFinishField = Field(FIELD_SIZE - 1, FIELD_SIZE - 1);
    mField = new GameField(mHge, mGameUtils, Point(FIELD_X, FIELD_Y), FIELD_SIZE, mFinishField);
    mPlayer = new Player(mHge, mField->GetCenter(mStartField));
    EmemiesInit();
    mCannons.push_back(std::shared_ptr<Cannon>(new Cannon(mHge, Point(CANNON1_X, CANNON1_Y),
                        Fireball::RIGHT, mField->GetBorderRect(), &mFireballs)));
    mCannons.push_back(std::shared_ptr<Cannon>(new Cannon(mHge, Point(CANNON2_X, CANNON2_Y),
                        Fireball::DOWN, mField->GetBorderRect(), &mFireballs)));
    mCannons.push_back(std::shared_ptr<Cannon>(new Cannon(mHge, Point(CANNON3_X, CANNON3_Y),
                        Fireball::LEFT, mField->GetBorderRect(), &mFireballs)));
    mCannons.push_back(std::shared_ptr<Cannon>(new Cannon(mHge, Point(CANNON4_X, CANNON4_Y),
                        Fireball::UP, mField->GetBorderRect(), &mFireballs)));
    mGameState = GAME_IS_PLAYED;
}

Game::~Game()
{
    delete mPlayer;
    delete mField;
	delete mGameUtils;
	for (int i=0; i<ENEMIES_NUMBER; i++) {
        delete mEnemies[i];
	}
	for(auto i=mFireballs.begin(); i!=mFireballs.end(); i++){(*i).reset();}
	for(auto i=mCannons.begin(); i!=mCannons.end(); i++){(*i).reset();}
}

void Game::Render()
{
	mField->Render();
	mPlayer->Render();
	for (int i = 0; i < ENEMIES_NUMBER; i++) {
        mEnemies[i]->Render();
	}
	for(auto i = mFireballs.begin(); i != mFireballs.end(); i++){(*i)->Render();}
	for(auto i = mCannons.begin(); i != mCannons.end(); i++){(*i)->Render();}
}

void Game::Frame()
{
    mPlayer->Frame();
    CalculatePlayer(mPlayer);
	Field currentPlayerField = mField->FindFieldByPoint(mPlayer->GetCurrentPosition());
	if (mFinishField == currentPlayerField) {
        mGameState = GAME_IS_PASSED;
        return;
    }
    std::vector<Field> playerFields;
	playerFields.push_back(currentPlayerField);
    for (int i = 0; i < ENEMIES_NUMBER; i++) {
        mEnemies[i]->Frame();
        CalculateEnemy(mEnemies[i]);
        Field currentEnemyField = mField->FindFieldByPoint(mEnemies[i]->GetCurrentPosition());
        if (currentEnemyField == currentPlayerField) {
            mGameState = GAME_IS_FAILED;
            return;
        }
        playerFields.push_back(currentEnemyField);
	}
	mField->FrameCalculate(playerFields);

	for (auto i = mCannons.begin(); i != mCannons.end(); ++i)
	{
	    (*i)->Frame();
    }
	for (auto i = mFireballs.begin(); i != mFireballs.end(); ++i)
    {
        (*i)->Frame();
        if ((*i)->IsOutOfBorder()) {
            i = mFireballs.erase(i);
            continue;
        }
        if((*i)->GetFireballRect()->Intersect(mPlayer->GetRect()))
        {
            mGameState = GAME_IS_FAILED;
            return;
        }
    }
	return;
}

void Game::CalculatePlayer(Player* aPlayer)
{
    Field currentPlayerField = mField->FindFieldByPoint(aPlayer->GetCurrentPosition());
    if (!aPlayer->IsMoving())
    {
        std::vector<Field> pathVector = mField->FindPath(currentPlayerField, mField->GetTarget());
        if (!pathVector.empty())
        {
            Field targetField = (*pathVector.begin());
            if (!mField->IsOccupied(targetField))
            {
                aPlayer->SetTarget(mField->GetCenter(targetField));
            }
        }
    }
}

void Game::CalculateEnemy(Enemy* aEnemy)
{
    Field currentEnemyField = mField->FindFieldByPoint(aEnemy->GetCurrentPosition());
    if (!aEnemy->IsMoving())
    {
        Field currentTarget = aEnemy->GetCurrentTarget();
        if (currentTarget == currentEnemyField)
        {
            currentTarget = aEnemy->FindNextPatrolTarget();
        }
        std::vector<Field> pathVector = mField->FindPath(currentEnemyField, currentTarget);
        if (!pathVector.empty())
        {
            Field targetField = (*pathVector.begin());
            if (!mField->IsOccupied(targetField))
            {
                aEnemy->SetTarget(mField->GetCenter(targetField));
            }
        }
    }
}

int Game::GetGameState()
{
    return mGameState;
}

void Game::EmemiesInit()
{
    mEnemies = new Enemy*[ENEMIES_NUMBER];
    Field PatrolPointCenters[ENEMIES_NUMBER];
    PatrolPointCenters[0] = Field(7, 7);
    PatrolPointCenters[1] = Field(7, 22);
    PatrolPointCenters[2] = Field(22, 7);
    PatrolPointCenters[3] = Field(22, 22);
    for (int i=0; i<ENEMIES_NUMBER; i++) {
        std::vector<Field> aPath;
        Field checkPoint1 = Field(PatrolPointCenters[i].row - 3, PatrolPointCenters[i].col - 3); aPath.push_back(checkPoint1);
        Field checkPoint2 = Field(PatrolPointCenters[i].row + 3, PatrolPointCenters[i].col - 3); aPath.push_back(checkPoint2);
        Field checkPoint3 = Field(PatrolPointCenters[i].row + 3, PatrolPointCenters[i].col + 3); aPath.push_back(checkPoint3);
        Field checkPoint4 = Field(PatrolPointCenters[i].row - 3, PatrolPointCenters[i].col + 3); aPath.push_back(checkPoint4);
        mEnemies[i] = new Enemy(mHge, mField->GetCenter(checkPoint1));
        mEnemies[i]->SetPatrolPath(aPath);
    }
}
