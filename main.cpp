#include <hge.h>
#include <hgefont.h>
#include "Game.h"

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define NOTIFY_X      512
#define NOTIFY_Y      384
#define NOTIFY_WIDTH  400
#define NOTIFY_HEIGHT 100

HGE*       hge=NULL;
Game*      game=NULL;
hgeSprite* menuTmp = NULL;
hgeSprite* gemeOver = NULL;
hgeSprite* gemeWin = NULL;
hgeQuad    backgroundQuad;
HTEXTURE backgroundTexture;
HTEXTURE menuTmpTexture;
HTEXTURE gemeOverTexture;
HTEXTURE gemeWinTexture;

enum {MENU, GAME, GAME_OVER, GAME_WIN};
int screen = MENU;

bool FrameFunc()
{
    if (hge->Input_GetKeyState(HGEK_ESCAPE))
    {
        return true;
    }
    switch (screen)
    {
        case MENU:
            if (hge->Input_GetKeyState(HGEK_ENTER))
            {
                if (game == NULL)
                {
                    game = new Game(hge);
                }
                screen = GAME;
            }
            break;
        case GAME:
            if (game != NULL)
            {
                game->Frame();
                if (game->GetGameState() == game->GAME_IS_FAILED)
                {
                    screen = GAME_OVER;
                }
                if (game->GetGameState() == game->GAME_IS_PASSED)
                {
                    screen = GAME_WIN;
                }
            }
            break;
        case GAME_OVER:
            if (hge->Input_GetKeyState(HGEK_SPACE))
            {
                screen = MENU;
                if (game != NULL)
                {
                    delete game;
                    game = NULL;
                }
            }
            break;
        case GAME_WIN:
            if (hge->Input_GetKeyState(HGEK_SPACE))
            {
                screen = MENU;
                if (game != NULL)
                {
                    delete game;
                    game = NULL;
                }
            }
            break;
    }
	return false;
}

bool RenderFunc()
{
    hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	hge->Gfx_RenderQuad(&backgroundQuad);
    switch (screen)
    {
        case MENU:
            menuTmp->Render(NOTIFY_X, NOTIFY_Y);
            break;
        case GAME:
            if (game != NULL)
            {
                game->Render();
            }
            break;
        case GAME_OVER:
            gemeOver->Render(NOTIFY_X, NOTIFY_Y);
            break;
        case GAME_WIN:
            gemeWin->Render(NOTIFY_X, NOTIFY_Y);
            break;
    }
	hge->Gfx_EndScene();
    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_LOGFILE, "hge.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Project");
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_HIDEMOUSE, false);
	hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if (hge->System_Initiate())
	{
        backgroundTexture = hge->Texture_Load("background.png");
        menuTmpTexture = hge->Texture_Load("menu.png");
        gemeOverTexture = hge->Texture_Load("gameOver.png");
        gemeWinTexture = hge->Texture_Load("gameWin.png");
        if (!backgroundTexture || !menuTmpTexture || !gemeOverTexture || !gemeWinTexture)
        {
            MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        }

        backgroundQuad.tex = backgroundTexture;
        backgroundQuad.blend = BLEND_DEFAULT;
        for (int i = 0; i < 4; i++)
        {
            backgroundQuad.v[i].col = 0xFFFFFFFF;
        }
        backgroundQuad.v[0].tx = 0.0;
        backgroundQuad.v[0].ty = 0.0;
        backgroundQuad.v[1].tx = 1.0;
        backgroundQuad.v[1].ty = 0.0;
        backgroundQuad.v[2].tx = 1.0;
        backgroundQuad.v[2].ty = 1.0;
        backgroundQuad.v[3].tx = 0.0;
        backgroundQuad.v[3].ty = 1.0;

        backgroundQuad.v[0].x = 0.0;
        backgroundQuad.v[0].y = 0.0;
        backgroundQuad.v[1].x = SCREEN_WIDTH;
        backgroundQuad.v[1].y = 0.0;
        backgroundQuad.v[2].x = SCREEN_WIDTH;
        backgroundQuad.v[2].y = SCREEN_HEIGHT;
        backgroundQuad.v[3].x = 0.0;
        backgroundQuad.v[3].y = SCREEN_HEIGHT;

        menuTmpTexture = hge->Texture_Load("menu.png");
        gemeOverTexture = hge->Texture_Load("gameOver.png");
        gemeWinTexture = hge->Texture_Load("gameWin.png");

        menuTmp = new hgeSprite(menuTmpTexture, 0, 0, NOTIFY_WIDTH, NOTIFY_HEIGHT);
        gemeOver = new hgeSprite(gemeOverTexture, 0, 0, NOTIFY_WIDTH, NOTIFY_HEIGHT);
        gemeWin = new hgeSprite(gemeWinTexture, 0, 0, NOTIFY_WIDTH, NOTIFY_HEIGHT);

        menuTmp->SetHotSpot(NOTIFY_WIDTH/2,NOTIFY_HEIGHT/2);
        gemeOver->SetHotSpot(NOTIFY_WIDTH/2,NOTIFY_HEIGHT/2);
        gemeWin->SetHotSpot(NOTIFY_WIDTH/2,NOTIFY_HEIGHT/2);

		hge->System_Start();
	}
	else
	{
	    MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}
	hge->Texture_Free(backgroundTexture);
	hge->Texture_Free(menuTmpTexture);
	hge->Texture_Free(gemeOverTexture);
	hge->Texture_Free(gemeWinTexture);

	delete menuTmp;
    delete gemeOver;
    delete gemeWin;
    if (game != NULL)
    {
        delete game;
    }
	hge->System_Shutdown();
	hge->Release();
	hge = NULL;
	return 0;
}
