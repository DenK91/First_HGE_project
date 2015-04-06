#ifndef GAME_H
#define GAME_H

#include <hgerect.h>
#include <math.h>
#include <memory>
#include "Utils.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "Fireball.h"
#include "Cannon.h"

#define FIELD_SIZE 30
#define FIELD_X 152
#define FIELD_Y 40

#define ENEMIES_NUMBER 4

#define CANNON1_X FIELD_X-1*FIELD_TEXTURE_SIZE
#define CANNON1_Y FIELD_Y+6*FIELD_TEXTURE_SIZE

#define CANNON2_X FIELD_X+6*FIELD_TEXTURE_SIZE
#define CANNON2_Y FIELD_Y-FIELD_TEXTURE_SIZE

#define CANNON3_X FIELD_X+30*FIELD_TEXTURE_SIZE
#define CANNON3_Y FIELD_Y+21*FIELD_TEXTURE_SIZE

#define CANNON4_X FIELD_X+21*FIELD_TEXTURE_SIZE
#define CANNON4_Y FIELD_Y+30*FIELD_TEXTURE_SIZE

class Game
{
    public:
        Game(HGE* aHGE);
        ~Game();
        void Render();
        void Frame();
        int GetGameState();
        enum {GAME_IS_PASSED, GAME_IS_FAILED, GAME_IS_PLAYED};
    protected:
    private:
        void EmemiesInit();
        void CalculatePlayer(Player* aPlayer);
        void CalculateEnemy(Enemy* aEnemy);
        HGE*       mHge;
        GameField* mField;
        Utils*     mGameUtils;
        Player*    mPlayer;
        Enemy**    mEnemies;
        Field      mStartField;
        Field      mFinishField;
        float      mMouseX;
        float      mMouseY;
        int        mGameState;
        std::vector<std::shared_ptr<Cannon>> mCannons;
        std::vector<std::shared_ptr<Fireball>> mFireballs;
};

#endif // GAME_H
