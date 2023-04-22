#pragma once

#include "Animation.h"
#include "Sprite.h"

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);

	static CAnimations* GetInstance();
};

//Subclass...
class CAnimationsFor2Stand
{
	static CAnimationsFor2Stand* __instance;

	unordered_map<int, LPANIMATION2STAND> animations;

public:
	void Add(int id, LPANIMATION2STAND ani);
	LPANIMATION2STAND Get(int id);

	static CAnimationsFor2Stand* GetInstance();
};