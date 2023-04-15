#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define MARIO_WIDTH 14

class CBrick : public CGameObject {
public: 
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
};

class CMario : public CGameObject
{
protected:
	float vx;
public:
	CMario(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};

class CGlassBrick : public CGameObject {
public:
	CGlassBrick(float x, float y) : CGameObject(x, y) {}
	void Update(DWORD dt) {}
	void Render();
};

class CCoin : public CGameObject {
public:
	CCoin(float x, float y) : CGameObject(x, y) {}
	void Update(DWORD dt) {}
	void Render();
};

class CClubba : public CGameObject
{
protected:
	float vx;
public:
	CClubba(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};


