#include <math.h>
#include "j1App.h"
#include "j1FadetoBlack.h"
#include "j1Window.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "p2Log.h"

j1FadetoBlack::j1FadetoBlack()
{
}

j1FadetoBlack::~j1FadetoBlack() {}

bool  j1FadetoBlack::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	uint width, height;
	App->win->GetWindowSize(width, height);
	screen = { 0,0,(int)width,(int)height };
	return true;
	//LOG("window width= %i / height= %i", width, height);
}

// Update to draw background
bool j1FadetoBlack::Update(float dt)
{
	if (current_step == fade_step::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::FADE_TO_BLACK:
	{
		if (now >= total_time)
		{
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::FADE_FROM_BLACK;
		}
	}
	break;

	case fade_step::FADE_FROM_BLACK:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::NONE;
	}
	break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);
	return true;
}

bool j1FadetoBlack::CleanUp()
{
	return true;
}

// fadetoblack makes the screen fade to black and fadefromblack makes it fade from black to the screen used at the time
bool j1FadetoBlack::fadetoBlack(float time)
{
	bool ret = false;

	if (current_step == fade_step::NONE)
	{
		current_step = fade_step::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		ret = true;
	}

	return ret;
}

bool j1FadetoBlack::fadefromblack(float time)
{
	bool ret = false;

	if (current_step == fade_step::NONE)
	{
		current_step = fade_step::FADE_FROM_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		ret = true;
	}

	return ret;
}


bool j1FadetoBlack::IsFading() const
{
	return current_step != fade_step::NONE;
}