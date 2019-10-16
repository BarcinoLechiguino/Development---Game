#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadetoBlack : public j1Module
{
public:
	j1FadetoBlack()
	{
		name.create("fade");
	}
	~j1FadetoBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(uint lvlnum, float time = 2.0f);
	bool Draw();
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
	float normal;


};

#endif //__MODULEFADETOBLACK_H__
