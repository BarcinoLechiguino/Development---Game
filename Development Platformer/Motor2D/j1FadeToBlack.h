#pragma once

#ifndef __J1FADETOBLACK_H__
#define  __J1FADETOBLACK_H__

#include "j1Module.h "
#include "SDL/include/SDL_rect.h "

class  j1FadetoBlack : public j1Module
{
public:
	j1FadetoBlack();
	~j1FadetoBlack();

	bool Start();
	bool Update(float id);
	bool CleanUp();
	bool fadetoBlack(float time = 2.0f);
	bool fadefromblack(float time = 2.0f);
	bool IsFading() const;


private:

	enum fade_step
	{
		NONE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK
	} current_step = fade_step::NONE;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif  