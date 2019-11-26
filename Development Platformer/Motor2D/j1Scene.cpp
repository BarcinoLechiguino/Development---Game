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
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Player1.h"
#include "j1Player2.h"
#include "Brofiler\Brofiler.h"

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
	bool ret;

	to_end = false;

	/*if (firstMap == true)
	{
		ret = App->map->Load(map_names.start->data->GetString());
		LOG("Map Name: %s", map_names.start->data->GetString());
	}
	else
	{
		ret = App->map->Load(map_names.start->next->data->GetString());
		LOG("Map Name: %s", map_names.start->next->data->GetString());
	}*/
	
	ret = App->map->Load(map_names.start->data->GetString());
	LOG("Map Name: %s", map_names.start->data->GetString());

	App->audio->PlayMusic(App->map->data.music_File.GetString());
	
	App->entityManager->CreatePlayers();								//THIS HERE
	/*App->entityManager->SpawnEnemies();*/								//If SpawnEnemies is called here then it should not be called in the PreUpdate()
	//App->entityManager->CreateEntity(ENTITY_TYPE::PLAYER);

	cam_debug_speed = App->render->cam.camera_debug_speed;

	return true;

}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)														//Receives dt as an argument.
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::LavenderBlush);
	//Camera Movement With Arrow Keys
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->render->camera.x += ceil(cam_debug_speed * dt);	//As the value is multiplied by dt, camera movement will be adjusted to the framerate.  (100 * 0.033s (30fps), 100 * 0.066s (60fps)...)
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->render->camera.x -= ceil(cam_debug_speed * dt);	//Ceil rounds up all the decimal values, returning the smallest integral value not less than the given value. 
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->render->camera.y += ceil(cam_debug_speed * dt);
	}	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->render->camera.y -= ceil(cam_debug_speed * dt);
	}
	
	//A spritesheet switch button just for the flavour. Not functional at the moment.
	/*if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (App->player1->player.switch_sprites == false || App->player2->p2.switch_sprites == false)
		{
			App->player1->player.switch_sprites == true;
			App->player2->p2.switch_sprites == true;
		}
		else
		{
			App->player1->player.switch_sprites == false;
			App->player2->p2.switch_sprites == false;
		}
	}*/

	// ---------------------------------------- DEBUG KEYS ----------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)			//Load First Level Key
	{
		//Load_lvl(0);

		//New
		App->fadescene->FadeToBlack("Test_map.tmx");
		/*App->entityManager->player->Restart();
		App->entityManager->player2->Restart();*/
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)			//Load Second Level Key
	{
		//Load_lvl(1);

		App->fadescene->FadeToBlack("Test_map_2.tmx");
		/*App->entityManager->player->Restart();
		App->entityManager->player2->Restart();*/
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)			//Restart Key
	{
		App->entityManager->player->Restart();
		App->entityManager->player2->Restart();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)			//Save Game Key
	{
		App->SaveGame();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)			//Load Game Key
	{
		App->LoadGame();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)			//Enable / Diable free camera movement Key
	{
		if (App->render->cam.camera_debug == true)
		{
			App->render->cam.camera_debug = false;
		}
		else
		{
			App->render->cam.camera_debug = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)			//Enabling / Disabling frame cap
	{
		if (App->framesAreCapped == true)
		{
			App->framesAreCapped = false;
		}
		else
		{
			App->framesAreCapped = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)			//Collider Debug Key
	{
		if (App->collisions->collider_debug == true)
		{
			App->collisions->collider_debug = false;
		}
		else
		{
			App->collisions->collider_debug = true;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)		//God Mode Key
	{
		if (App->entityManager->player->player.GodMode)
		{
			App->entityManager->player->player.GodMode = false;
			App->entityManager->player2->player.GodMode = false;
			App->entityManager->player->player.airborne = true;
			App->entityManager->player2->player.airborne = true;
		}
		else
		{
			App->entityManager->player->player.GodMode = true;
			App->entityManager->player2->player.GodMode = true;
		}
	}

	//Volume Change
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		App->audio->general_volume += 5;
		App->audio->SetVolumeMusic();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		App->audio->general_volume -= 5;
		App->audio->SetVolumeMusic();
	}

	App->map->Draw();


	//Technical title
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d", App->map->data.width, App->map->data.height, App->map->data.tile_width, App->map->data.tile_height, App->map->data.tilesets.count());
	App->win->SetTitle(title.GetString());*/

	return true;
}


// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::LavenderBlush);
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	App->collisions->CleanUp();								//Deletes all colliders that were loaded for this scene / map.
	App->entityManager->DestroyEntities();					//Destroys all non-player entities.
	App->map->CleanUp();									//Deletes everything related with the map from memory. (Tilesets, Layers and ObjectGroups)
	//App->audio->CleanUp();

	if (App->entityManager->player != nullptr)
	{
		App->entityManager->player->CleanUp();				//Deletes all data related to P1. 
	}
	if (App->entityManager->player2 != nullptr)
	{
		App->entityManager->player2->CleanUp();				//Deletes all data related to P2.
	}

	/*if (firstMap == true)									//Resets the booleans depending on which map was loaded  / deleted.
	{
		firstMap	= false;
		secondMap	= true;
	}
	else
	{
		firstMap	= true;
		secondMap	= false;
	}*/

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



