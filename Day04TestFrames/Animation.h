#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"


using namespace std;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y);
};

typedef CAnimation* LPANIMATION;

//Subclass...

class CAnimationFor2Stand
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME2STAND> frames;
public:
	CAnimationFor2Stand(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId1, int spriteId2, DWORD time = 0);
	void Render(float x, float y, float spriteHeight);
};

typedef CAnimationFor2Stand* LPANIMATION2STAND;