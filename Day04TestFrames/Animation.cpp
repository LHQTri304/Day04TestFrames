#include "Animation.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}

	}

	frames[currentFrame]->GetSprite()->Draw(x, y);
}

//Subclass...
void CAnimationFor2Stand::Add(int spriteId1, int spriteId2, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite1 = CSprites::GetInstance()->Get(spriteId1);
	LPSPRITE sprite2 = CSprites::GetInstance()->Get(spriteId2);
	LPANIMATION_FRAME2STAND frame = new CAnimationFrameFor2Stand(sprite1, sprite2, t);
	frames.push_back(frame);
}
/*
void CAnimationFor2Stand::AddBottom(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME2STAND frame = new CAnimationFrameFor2Stand(sprite, t);
	framesBottom.push_back(frame);
}*/


void CAnimationFor2Stand::Render(float x, float y, float spriteHeight)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}

	}

	frames[currentFrame]->GetSprite1()->Draw(x, y);
	frames[currentFrame]->GetSprite2()->Draw(x, y + spriteHeight);
}