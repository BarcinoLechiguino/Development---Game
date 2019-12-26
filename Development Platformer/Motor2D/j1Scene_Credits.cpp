#include "j1App.h"
#include "j1Scene_Credits.h"
#include "j1SceneMenu.h"
#include "p2Log.h"
#include "j1Input.h"
#include "UiItem.h"
#include "j1Render.h"
#include "UiItem_Label.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1FadeScene.h"
#include "j1Window.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1Scene_Credits::j1Scene_Credits() : j1Module()
{
	name.create("scene_menu");
}

// Destructor
j1Scene_Credits::~j1Scene_Credits()
{}

// Called before render is available
bool j1Scene_Credits::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Credits");
	bool ret = true;
	path.create(config.child("path").child_value());

	return ret;
}

// Called before the first frame
bool j1Scene_Credits::Start()
{
	bool ret = true;

	return ret;
}

// Called each loop iteration
bool j1Scene_Credits::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_SceneCredits", Profiler::Color::CornflowerBlue);

	return true;
}

// Called each loop iteration
bool j1Scene_Credits::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneCredits", Profiler::Color::DarkKhaki);

	bool ret = true;

	return ret;
}

// Called each loop iteration
bool j1Scene_Credits::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneCredits", Profiler::Color::MediumVioletRed);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}
		
	return ret;
}

// Called before quitting
bool j1Scene_Credits::CleanUp()
{
	LOG("Freeing Scene Menu");
	button_list.clear();
	return true;
}

