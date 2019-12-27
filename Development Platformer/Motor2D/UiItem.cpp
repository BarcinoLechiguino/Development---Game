#include "UiItem.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "j1FadeScene.h"

bool UI_Item::PreUpdate(float dt)
{
	switch (state)
	{
	case UI_State::IDLE:
		if (OnHover())
		{
			state = UI_State::HOVER;
		}
		break;

	case UI_State::HOVER:
		if (!OnHover())
		{
			state = UI_State::IDLE;
		}
		if (OnClick())
		{
			state = UI_State::CLICK;
		}
		break;

	case UI_State::CLICK:
		if (!OnClick())
		{
			state = UI_State::HOVER;
		}
		break;

	default:
		break;
	}
	return true;
}

bool UI_Item::Update(float dt)
{
	int x, y;
	App->input->GetMousePosition(x, y);


	if (static_object)
	{
		if (!drag)
			position = { pos.x - App->render->camera.x / (int)App->win->GetScale(), pos.y - App->render->camera.y / (int)App->win->GetScale() };
		mouse_position = { x - App->render->camera.x / (int)App->win->GetScale(),y - App->render->camera.y / (int)App->win->GetScale() };
	}
	else if (!static_object)
	{
		if (!drag)
			position = { pos.x , pos.y };
		mouse_position = { x - App->render->camera.x / (int)App->win->GetScale(), y - App->render->camera.y / (int)App->win->GetScale() };
	}

	return true;
}

bool UI_Item::OnClick()
{
	bool ret = false;

	if (!App->fadescene->IsChanging() && OnHover())
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->audio->PlayFx(App->gui->fx_buton_pressed);
			ret = true;
		}
	}
	return ret;
}

bool UI_Item::OnClick2()
{
	bool ret = false;

	if (!App->fadescene->IsChanging() && OnHover())
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			
			ret = true;
		}
		else if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->audio->PlayFx(App->gui->fx_buton_pressed);
		}
	}
	return ret;
}

