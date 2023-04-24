#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

//#define MARIO_WIDTH 14
#define MARIO_START_Y 68.0f

#define DISTANCE_BRICK_ROWS	55

#define REVERSE_COIN_START_Y 178.0f

#define CLUBBA_START_X 300.0f
#define CLUBBA_START_Y 178.0f

class CBrick : public CGameObject {
public: 
	CBrick(float x, float y, float r) : CGameObject(x, y, r) {}
	void Render();
	void Update(DWORD dt) {}
};

class CMario : public CGameObject
{
protected:
	float vx;
public:
	CMario(float x, float y, float r, float vx);
	void ReverseVX(int signal) { if(signal) vx = -vx; }
	void Update(DWORD dt);
	void Render();
};

class CGlassBrick : public CGameObject {
public:
	CGlassBrick(float x, float y, float r) : CGameObject(x, y, r) {}
	void Update(DWORD dt) {}
	void Render();
};

class CCoin : public CGameObject {
public:
	CCoin(float x, float y, float r) : CGameObject(x, y, r) {}
	void Update(DWORD dt) {}
	void Render();
};

class CClubba : public CGameObject
{
protected:
	float vx;
public:
	CClubba(float x, float y, float r, float vx);
	void Update(DWORD dt);
	void Render();
};

class CDoor : public CGameObject {
	float spriteHeight;
public:
	CDoor(float x, float y, float r, float spriteHeight) : CGameObject(x, y, r) { this->spriteHeight = spriteHeight; }
	void Update(DWORD dt) {}
	void Render();
};


