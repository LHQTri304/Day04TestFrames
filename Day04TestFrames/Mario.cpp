#include "Game.h"
#include "Mario.h"

CMario::CMario(float x, float y, float r, float vx):CGameObject(x, y, r)
{
	this->vx = vx;
};

void CMario::Update(DWORD dt)
{
	x += vx*dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	if (x <= 0 || x >= BackBufferWidth) {

		vx = -vx;

		y += DISTANCE_BRICK_ROWS;
		if (y >= BackBufferHeight)
			y = MARIO_START_Y;
		
		if (x <= 0)
		{
			x = 10;
		}
		else if (x >= BackBufferWidth)
		{
			x = (float)(BackBufferWidth) -10;
		}
	}
}

void CMario::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx>0) ani = CAnimations::GetInstance()->Get(500);
	else ani = CAnimations::GetInstance()->Get(501);

	ani->Render(x, y);
}

void CBrick::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(510);

	ani->Render(x, y);

}

void CGlassBrick::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(520);

	ani->Render(x, y);

}

void CCoin::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(530);

	ani->Render(x, y);

}

//*CClubba
CClubba::CClubba(float x, float y, float r, float vx) :CGameObject(x, y, r)
{
	this->vx = vx;
};

void CClubba::Update(DWORD dt)
{
	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth) {

		vx = -vx;

		y -= DISTANCE_BRICK_ROWS;
		if (y <= 67)	//The highest place is the movable Clubba which is 68
			y = CLUBBA_START_Y;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth)
		{
			x = (float)(BackBufferWidth);
		}
	}
}

void CClubba::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(540);
	else ani = CAnimations::GetInstance()->Get(541);

	ani->Render(x, y);
}

//Door
void CDoor::Render() {

	LPANIMATION2STAND ani = CAnimationsFor2Stand::GetInstance()->Get(100);

	ani->Render(x, y, spriteHeight);
}
