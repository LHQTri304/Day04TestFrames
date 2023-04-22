#include "Animations.h"

CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}



//Subclass...
CAnimationsFor2Stand* CAnimationsFor2Stand::__instance = NULL;

CAnimationsFor2Stand* CAnimationsFor2Stand::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationsFor2Stand();
	return __instance;
}

void CAnimationsFor2Stand::Add(int id, LPANIMATION2STAND ani)
{
	animations[id] = ani;
}

LPANIMATION2STAND CAnimationsFor2Stand::Get(int id)
{
	return animations[id];
}