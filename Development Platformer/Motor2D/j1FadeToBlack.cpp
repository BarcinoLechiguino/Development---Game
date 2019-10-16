#include <math.h>
#include "j1App.h"
#include "j1FadetoBlack.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Player1.h"
#include "j1Player2.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "p2Log.h"

j1FadetoBlack::~j1FadetoBlack() {}

bool  j1FadetoBlack::Start()
{
	screen = { 0, 0, App->render->camera.w , App->render->camera.h };
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update to draw background
bool j1FadetoBlack::Update(float dt)
{
	if (current_step == fade_step::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	normal = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::FADE_TO_BLACK:
	{
		if (now >= total_time)
		{

			App->map->Disable();
			App->map->Enable();
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::FADE_FROM_BLACK;
		}
	} break;

	case fade_step::FADE_FROM_BLACK:
	{
		App->player1->fading = false;
		normal = 1.0f - normal;

		if (now >= total_time)
			current_step = fade_step::NONE;
	} break;
	}
	return true;
}

bool j1FadetoBlack::Draw()
{
	// Finally render the black square with alpha on the screen
	App->render->DrawQuad(screen, 0, 0, 0, (Uint8)(normal * 255.0f), true, false);
	return true;
}

// fadetoblack makes the screen fade to black and fadefromblack makes it fade from black to the screen used at the time
bool j1FadetoBlack::FadeToBlack(uint lvlnum, float time)
{
	bool ret = false;

	if (current_step == fade_step::NONE)
	{
		App->player1->fading = true;
		App->player2->fading = true;
		current_step = fade_step::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		App->map->num_thismaplvl = lvlnum;
		App->map->activateScene(lvlnum);


		ret = true;
	}

	return ret;
}