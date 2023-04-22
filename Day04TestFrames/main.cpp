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

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_ENEMIES_RtL TEXTURES_DIR "\\enemiesRtL.png"
#define TEXTURE_PATH_ENEMIES_LtR TEXTURES_DIR "\\enemiesLtR.png"

CMario *mario;
#define MARIO_RADIUS 8.0f
#define MARIO_START_X 10.0f
#define MARIO_START_Y 178.0f
#define MARIO_START_VX 0.3f

CBrick *brick;

CGlassBrick* gbrick;
#define GBRICK_X 0.0f
#define GBRICK_Y 200.0f
#define GBRICK_WIDTH 16
#define NUM_OF_BRICKS 20
#define DISTANCE_BRICK_ROWS	55

CCoin* coin;
#define COIN_RADIUS 8.0f
#define COIN_START_X SCREEN_WIDTH/2
#define COIN_START_Y 178.0f

CClubba* clubba;
#define CLUBBA_START_X 300.0f
#define CLUBBA_START_Y 150.0f
#define CLUBBA_START_VX 0.1f

CDoor* doors;
#define DOOR_SPRITE_HEIGHT 16.0f

vector<CGlassBrick*> gbricks1;
vector<CGlassBrick*> gbricks2;
vector<CGlassBrick*> gbricks3;
vector<CGlassBrick*> gbricks4;

//Check collide function
int CheckCollideObject(CGameObject* Obj1, CGameObject* Obj2)
{
	float RandCoinSpawnX = (rand() % (SCREEN_WIDTH-20) + 10);

	if (abs(Obj1->GetX() - Obj2->GetX()) <= Obj1->GetR() + Obj2->GetR()
		&& abs(Obj1->GetY() - Obj2->GetY()) <= Obj1->GetR() + Obj2->GetR())
	{
		Obj2->SetPosition(RandCoinSpawnX, Obj2->GetY() - DISTANCE_BRICK_ROWS);
	}
	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
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
	ani->Add(50001,500);
	ani->Add(50002,500);
	animations->Add(540, ani);

	ani = new CAnimation(100);
	ani->Add(50011,500);
	ani->Add(50012,500);
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

	
	//Row of bricks...
	float nextBrickX = GBRICK_X;
	for (int i = 0; i <= NUM_OF_BRICKS; i++)
	{
		gbricks1.push_back(new CGlassBrick(nextBrickX, GBRICK_Y, 0));
		gbricks2.push_back(new CGlassBrick(nextBrickX, GBRICK_Y - DISTANCE_BRICK_ROWS, 0));
		gbricks3.push_back(new CGlassBrick(nextBrickX, GBRICK_Y - DISTANCE_BRICK_ROWS * 2, 0));
		gbricks4.push_back(new CGlassBrick(nextBrickX, GBRICK_Y - DISTANCE_BRICK_ROWS * 3, 0));
		nextBrickX += GBRICK_WIDTH;
	}
	

	mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_RADIUS, MARIO_START_VX);
	brick = new CBrick(100.0f, 100.0f, 0);
	gbrick = new CGlassBrick(200.0f, 100.0f, 0);
	coin = new CCoin(COIN_START_X, COIN_START_Y, COIN_RADIUS);
	clubba = new CClubba(CLUBBA_START_X, CLUBBA_START_Y, 0, CLUBBA_START_VX);
	doors = new CDoor(280.0f, 100.0f, DOOR_SPRITE_HEIGHT, 0);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	mario->Update(dt);
	coin->Update(dt);
	CheckCollideObject(mario, coin);
	clubba->Update(dt);
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

		//brick->Render();
		//gbrick->Render();
		coin->Render();

		mario->Render();
		//clubba->Render();

		//doors->Render();

		for (int i = 0; i < NUM_OF_BRICKS; i++)
		{
			gbricks1[i]->Render();
			gbricks2[i]->Render();
			gbricks3[i]->Render();
			gbricks4[i]->Render();
		}

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