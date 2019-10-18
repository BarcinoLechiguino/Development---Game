#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1FadeScene.h"
#include "j1Scene.h"
#include "j1Player1.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	fade_time = config.child("fade_time").attribute("value").as_float();

	for (pugi::xml_node map = config.child("map_name"); map; map = map.next_sibling("map_name"))
	{
		p2SString* data = new p2SString;

		data->create(map.attribute("name").as_string());
		map_names.add(data);
	}

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	to_end = false;
	bool ret = App->map->Load(map_names.start->data->GetString());
	App->audio->PlayMusic(App->map->data.music_File.GetString());
	LOG("Boi: %s", map_names.start->data->GetString());
	return true;

}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	//{
	//	App->SaveGame("Save_Game.xml");
	//	if (tutorial_map)
	//		map_saved = true;
	//	else
	//		map_saved = false;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	//	load = true;
	//if (load)
	//{
	//	if (tutorial_map && map_saved)
	//	{
	//		App->LoadGame("Save_Game.xml");
	//		App->fade->fadetoBlack();
	//		/*App->audio->PlayMusic();*/

	//	}

	//	if (!tutorial_map && !map_saved)
	//	{
	//		App->LoadGame("Save_Game.xml");
	//		App->fade->fadetoBlack();
	//		/*App->audio->PlayMusic();*/
	//	}
	//	load = false;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Load_lvl(0);

	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		Load_lvl(1);

	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		Load_lvl(1);

	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->LoadGame();

	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->SaveGame();
		
	App->map->Draw();

		return true;
}


// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (to_end)
	{
		if (currentMap < map_names.count() - 1)
			ret = App->fadescene->ChangeMap(++currentMap, fade_time);
		else
			currentMap = 0, ret = App->fadescene->ChangeMap(currentMap, fade_time);


		to_end = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{

	if (currentMap != data.child("currentMap").attribute("num").as_int())
	{
		LOG("Calling switch maps");
		currentMap = data.child("currentMap").attribute("num").as_int();
		App->map->SwitchMaps(map_names[data.child("currentMap").attribute("num").as_int()]);

	}
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	data.append_child("currentMap").append_attribute("num") = currentMap;
	return true;
}

bool j1Scene::Load_lvl(int time)
{
	App->map->SwitchMaps(map_names[time]);
	return false;
}

