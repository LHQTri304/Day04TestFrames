#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"


using namespace std;

class CGameObject
{
protected:
	float x; 
	float y;

	float r = 0;	//radius
public: 
	CGameObject(float x, float y, float r);

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	float GetX() { return x; }
	float GetY() { return y; }
	float GetR() { return r; }

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};

