#pragma once

#include "Sprite.h"

/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

//Subclass...
class CAnimationFrameFor2Stand
{
	LPSPRITE sprite1;
	LPSPRITE sprite2;
	DWORD time;

public:
	CAnimationFrameFor2Stand(LPSPRITE sprite1, LPSPRITE sprite2, int time) { this->sprite1 = sprite1; this->sprite2 = sprite2; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite1() { return sprite1; }
	LPSPRITE GetSprite2() { return sprite2; }
};

typedef CAnimationFrameFor2Stand* LPANIMATION_FRAME2STAND;