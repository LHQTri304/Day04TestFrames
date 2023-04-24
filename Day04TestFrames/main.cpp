/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "debug.h"
#include "Game.h"
#include "Textures.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Animation.h"
#include "Animations.h"


#include "Mario.h"


#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255,0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY_R 10
#define ID_TEX_ENEMY_L 11
#define ID_TEX_MISC 20
#define ID_TEX_VICTORY 30

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_ENEMIES_RtL TEXTURES_DIR "\\enemiesRtL.png"
#define TEXTURE_PATH_ENEMIES_LtR TEXTURES_DIR "\\enemiesLtR.png"
#define TEXTURE_PATH_VICTORY TEXTURES_DIR "\\victory.png"

CMario *mario;
#define MARIO_RADIUS 8.0f
#define MARIO_START_X 10.0f
#define MARIO_START_Y 68.0f
#define MARIO_START_VX 0.2f
float RandMarioSpawnY[3] = {
	MARIO_START_Y,
	MARIO_START_Y + DISTANCE_BRICK_ROWS,
	MARIO_START_Y + DISTANCE_BRICK_ROWS * 2};

CBrick *brick;

CGlassBrick* gbrick;
#define GBRICK_X 0.0f
#define GBRICK_Y 200.0f
#define GBRICK_WIDTH 16
#define NUM_OF_BRICKS 20
#define DISTANCE_BRICK_ROWS	55

CCoin* coinScore;
#define COIN_RADIUS 8.0f
#define COIN_START_X MARIO_START_X
#define COIN_START_Y MARIO_START_Y
#define NUM_OF_COINS 10

CClubba* clubba;
#define CLUBBA_START_X 300.0f
#define CLUBBA_START_Y 178.0f
#define CLUBBA_START_VX -0.12f

CDoor* door;
#define DOOR_SPRITE_HEIGHT 16.0f


vector<CGlassBrick*> gbricks1;
vector<CGlassBrick*> gbricks2;
vector<CGlassBrick*> gbricks3;

vector<CCoin*> coins;

vector<LPSPRITE> numbersRed;
vector<LPSPRITE> numbersBlack;
vector<LPSPRITE> stuff;
int baseLifes = 10;
int maxScore = 50;	//Do not set it over 999
int score = -NUM_OF_COINS;
int lifes = maxScore / NUM_OF_COINS;
//int victory = 0;	// -1: lose | 1: win
int running = 1;	// 1: game running | 0: end game
int pressSpaceBar = 0;
int numIndex[100];	//0-2 for score | 3-5 for maxScore | 6,7 for lifes


//Check collide function
int CheckCollideObject(CGameObject* Obj1, CGameObject* Obj2)
{
	if (abs(Obj1->GetX() - Obj2->GetX()) <= Obj1->GetR() + Obj2->GetR()
		&& abs(Obj1->GetY() - Obj2->GetY()) <= Obj1->GetR() + Obj2->GetR())
		return 1;
	return 0;
}

void CollideMarioCoin(CGameObject* mario, CGameObject* coin)
{
	float RandCoinSpawnX = (rand() % (SCREEN_WIDTH - 20) + 10);
	int SpawnIndex = (rand() % 3);

	if (CheckCollideObject(mario, coin))
	{
		coin->SetPosition(RandCoinSpawnX, RandMarioSpawnY[SpawnIndex]);	//use mario's for convenience
		score++;
	}
}

void CollideMarioClubba(CGameObject* mario, CGameObject* clubba)
{
	int SpawnIndex = (rand() % 3);

	if (CheckCollideObject(mario, clubba))
	{
		mario->SetPosition(MARIO_START_X, RandMarioSpawnY[SpawnIndex]);
		lifes--;
	}
}

//Restart game funcsion...
void RestartGame()
{
	score = -NUM_OF_COINS;
	lifes = maxScore / NUM_OF_COINS;
	running = 1;
	pressSpaceBar = 0;

	mario->SetPosition(MARIO_START_X, MARIO_START_Y);
	clubba->SetPosition(CLUBBA_START_X, CLUBBA_START_Y);
	for (int i = 0; i < NUM_OF_COINS; i++)
		coins[i]->SetPosition(COIN_START_X, COIN_START_Y);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
		{
			pressSpaceBar = 1;
			break;
		}
		pressSpaceBar = 0;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	//textures->Add(ID_ENEMY_TEXTURE, TEXTURE_PATH_ENEMIES, D3DCOLOR_XRGB(156, 219, 239));
	textures->Add(ID_TEX_ENEMY_R, TEXTURE_PATH_ENEMIES_RtL);
	textures->Add(ID_TEX_ENEMY_L, TEXTURE_PATH_ENEMIES_LtR);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_VICTORY, TEXTURE_PATH_VICTORY);


	CSprites * sprites = CSprites::GetInstance();
	
	//Mario...
	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// readline => id, left, top, right 

	sprites->Add(10001, 246, 154, 259, 181, texMario);
	sprites->Add(10002, 275, 154, 290, 181, texMario);
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	sprites->Add(10011, 186, 154, 200, 181, texMario);
	sprites->Add(10012, 155, 154, 171, 181, texMario);
	sprites->Add(10013, 125, 154, 141, 181, texMario);

	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);



	ani = new CAnimation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);


	//Question brick...
	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 300, 117, 317, 133, texMisc);
	sprites->Add(20002, 318, 117, 335, 133, texMisc);
	sprites->Add(20003, 336, 117, 353, 133, texMisc);
	sprites->Add(20004, 354, 117, 371, 133, texMisc);

	ani = new CAnimation(100);
	ani->Add(20001,1000);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(510, ani);


	//Glass brick...
	sprites->Add(30001, 300, 135, 317, 151, texMisc);
	sprites->Add(30002, 318, 135, 335, 151, texMisc);
	sprites->Add(30003, 336, 135, 353, 151, texMisc);
	sprites->Add(30004, 354, 135, 371, 151, texMisc);

	ani = new CAnimation(100);
	ani->Add(30001, 1000);	//id , time
	ani->Add(30002);
	ani->Add(30003);
	ani->Add(30004);
	animations->Add(520, ani);


	//Coin...
	sprites->Add(40001, 300, 171, 315, 186, texMisc);
	sprites->Add(40002, 300, 99, 315, 115, texMisc);
	sprites->Add(40003, 318, 99, 335, 115, texMisc);
	sprites->Add(40004, 336, 99, 353, 115, texMisc);

	ani = new CAnimation(100);
	ani->Add(40001, 100);	//id , time
	ani->Add(40002);
	ani->Add(40003);
	ani->Add(40004);
	animations->Add(530, ani);

	//Clubba...
	LPTEXTURE texEnemyL = textures->Get(ID_TEX_ENEMY_L);
	LPTEXTURE texEnemyR = textures->Get(ID_TEX_ENEMY_R);

	sprites->Add(50001, 436, 129, 451, 155, texEnemyL);
	sprites->Add(50002, 458, 130, 473, 155, texEnemyL);

	sprites->Add(50011, 6, 130, 21, 155, texEnemyR);
	sprites->Add(50012, 28, 129, 43, 155, texEnemyR);

	ani = new CAnimation(100);
	ani->Add(50001,250);
	ani->Add(50002,250);
	animations->Add(540, ani);

	ani = new CAnimation(100);
	ani->Add(50011,250);
	ani->Add(50012,250);
	animations->Add(541, ani);

	//Door...
	CAnimationsFor2Stand* animations2 = CAnimationsFor2Stand::GetInstance();
	LPANIMATION2STAND ani2;

	sprites->Add(50001, 282, 99, 297, 114, texMisc);
	sprites->Add(50011, 282, 117, 297, 132, texMisc);

	sprites->Add(50002, 282, 135, 297, 150, texMisc);
	sprites->Add(50012, 282, 153, 297, 168, texMisc);

	sprites->Add(50003, 282, 171, 297, 186, texMisc);
	sprites->Add(50013, 282, 189, 297, 204, texMisc);

	sprites->Add(50004, 282, 207, 297, 222, texMisc);
	sprites->Add(50014, 282, 225, 297, 240, texMisc);

	ani2 = new CAnimationFor2Stand(100);
	ani2->AddTop(50001, 50011, 1000);	//id1 , id2, time
	ani2->AddTop(50002, 50012);
	ani2->AddTop(50003, 50013 );
	ani2->AddTop(50004, 50014, 500);
	ani2->AddTop(50003, 50013);
	ani2->AddTop(50002, 50012);
	animations2->Add(100, ani2);

	//Red numbers...
	numbersRed.push_back(new CSprite(60000, 549, 174, 556, 187, texMisc));
	numbersRed.push_back(new CSprite(60001, 558, 174, 565, 187, texMisc));
	numbersRed.push_back(new CSprite(60002, 567, 174, 574, 187, texMisc));
	numbersRed.push_back(new CSprite(60003, 576, 174, 583, 187, texMisc));
	numbersRed.push_back(new CSprite(60004, 585, 174, 592, 187, texMisc));
	numbersRed.push_back(new CSprite(60005, 549, 189, 556, 202, texMisc));
	numbersRed.push_back(new CSprite(60006, 558, 189, 565, 202, texMisc));
	numbersRed.push_back(new CSprite(60007, 567, 189, 574, 202, texMisc));
	numbersRed.push_back(new CSprite(60008, 576, 189, 583, 202, texMisc));
	numbersRed.push_back(new CSprite(60009, 585, 189, 592, 202, texMisc));

	//Black numbers...
	numbersBlack.push_back(new CSprite(60000, 496, 225, 503, 238, texMisc));
	numbersBlack.push_back(new CSprite(60001, 505, 225, 512, 238, texMisc));
	numbersBlack.push_back(new CSprite(60002, 514, 225, 521, 238, texMisc));
	numbersBlack.push_back(new CSprite(60003, 523, 225, 530, 238, texMisc));
	numbersBlack.push_back(new CSprite(60004, 532, 225, 539, 238, texMisc));
	numbersBlack.push_back(new CSprite(60005, 496, 239, 503, 252, texMisc));
	numbersBlack.push_back(new CSprite(60006, 505, 239, 512, 252, texMisc));
	numbersBlack.push_back(new CSprite(60007, 514, 239, 521, 252, texMisc));
	numbersBlack.push_back(new CSprite(60008, 523, 239, 530, 252, texMisc));
	numbersBlack.push_back(new CSprite(60009, 532, 239, 539, 252, texMisc));

	//Other...
	LPTEXTURE texVictory = textures->Get(ID_TEX_VICTORY);
	stuff.push_back(new CSprite(70001, 215, 120, 230, 135, texMario));
	stuff.push_back(new CSprite(70002, 456, 266, 521, 283, texMisc));
	stuff.push_back(new CSprite(70003, 0, 0, 220, 95, texVictory));
	stuff.push_back(new CSprite(70004, 553, 116, 560, 131, texMisc));


	
	//Row of bricks...
	float nextBrickX = GBRICK_X;
	for (int i = 0; i <= NUM_OF_BRICKS; i++)
	{
		gbricks1.push_back(new CGlassBrick(nextBrickX, GBRICK_Y, 0));
		gbricks2.push_back(new CGlassBrick(nextBrickX, GBRICK_Y - DISTANCE_BRICK_ROWS, 0));
		gbricks3.push_back(new CGlassBrick(nextBrickX, GBRICK_Y - DISTANCE_BRICK_ROWS * 2, 0));
		nextBrickX += GBRICK_WIDTH;
	}

	//More coin...
	for (int i = 0; i < NUM_OF_COINS; i++)
	{
		coins.push_back(new CCoin(COIN_START_X, COIN_START_Y, COIN_RADIUS));
	}

	

	mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_RADIUS, MARIO_START_VX);
	//brick = new CBrick(100.0f, 100.0f, 0);
	//gbrick = new CGlassBrick(200.0f, 100.0f, 0);
	coinScore = new CCoin(10.0f, 10.0f, 0);
	clubba = new CClubba(CLUBBA_START_X, CLUBBA_START_Y, 0, CLUBBA_START_VX);
	//door = new CDoor(280.0f, 100.0f, DOOR_SPRITE_HEIGHT, 0);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	if (running)
	{
		mario->ReverseVX(pressSpaceBar);
		pressSpaceBar = 0;
		mario->Update(dt);

		for (int i = 0; i < NUM_OF_COINS; i++)
		{
			coins[i]->Update(dt);
			CollideMarioCoin(mario, coins[i]);
		}

		clubba->Update(dt);
		CollideMarioClubba(mario, clubba);

		//Counting score
		numIndex[0] = (score % 100) % 10;
		numIndex[1] = (score % 100) / 10;
		numIndex[2] = score / 100;

		//Counting max score
		numIndex[3] = (maxScore % 100) % 10;
		numIndex[4] = (maxScore % 100) / 10;
		numIndex[5] = maxScore / 100;

		//Counting life
		numIndex[6] = (lifes % 100) % 10;
		numIndex[7] = (lifes % 100) / 10;



		if (score >= 999 || score >= maxScore || lifes <= 0)	//>=999 to avoid error
			running = 0;
	}
	else if (pressSpaceBar)
	{
		RestartGame();
		running = 1;
	}
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		if(running)		
		{
			//brick->Render();

			//door->Render();

			for (int i = 0; i < NUM_OF_BRICKS; i++)
			{
				gbricks1[i]->Render();
				gbricks2[i]->Render();
				gbricks3[i]->Render();
			}


			for (int i = 0; i < NUM_OF_COINS; i++)
			{
				coins[i]->Render();
			}

			mario->Render();
			clubba->Render();
		}

		if (score >= 999 || score >= maxScore)	//>=999 to avoid error
		{
			score = maxScore;
			stuff[2]->Draw(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		}
		coinScore->Render();
		numbersRed[numIndex[2]]->Draw(25.0f, 10.0f);
		numbersRed[numIndex[1]]->Draw(35.0f, 10.0f);
		numbersRed[numIndex[0]]->Draw(45.0f, 10.0f);
		stuff[3]->Draw(55.0f, 10.0f);
		numbersRed[numIndex[5]]->Draw(65.0f, 10.0f);
		numbersRed[numIndex[4]]->Draw(75.0f, 10.0f);
		numbersRed[numIndex[3]]->Draw(85.0f, 10.0f);


		if (lifes <= 0)
		{
			lifes = 0;
			stuff[1]->Draw(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		}
		stuff[0]->Draw(270.0f, 10.0f);
		numbersBlack[numIndex[7]]->Draw(285.0f, 10.0f);
		numbersBlack[numIndex[6]]->Draw(295.0f, 10.0f);



		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 316, 133);


		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame *game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}