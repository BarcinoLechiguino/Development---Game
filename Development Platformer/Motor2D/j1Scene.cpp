#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "UiItem.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1FadeScene.h"
#include "j1Scene.h"
#include "j1SceneMenu.h"
#include "j1Collisions.h"
#include "j1Gui.h"
#include "UiItem_Label.h"
#include "j1EntityManager.h"
#include "j1Player1.h"
#include "j1App.h"
#include "j1Player2.h"
#include "j1Pathfinding.h"

#include "Brofiler\Brofiler.h"

//#include "mmgr/mmgr.h"

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

	bool ret = true;
	
	fade_time = config.child("fade_time").attribute("value").as_float();

	for (pugi::xml_node map = config.child("map_name"); map; map = map.next_sibling("map_name"))
	{
		p2SString* data = new p2SString;							//Memory Leak. Should delete un CleanUp

		data->create(map.attribute("name").as_string());
		map_names.add(data);
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret;

	to_end = false;
	
	firstMap	= true;
	secondMap	= false;

	ret = App->map->Load(map_names.start->data->GetString());
	LOG("Map Name: %s", map_names.start->data->GetString());

	App->audio->PlayMusic(App->map->data.music_File.GetString());

	//if (App->entityManager->player == nullptr && App->entityManager->player2) == nullptr) { App->entityManager->CreatePlayers(); } //Use this if App->Scene->Start() is called in the ChangeMap() function.
	App->entityManager->CreatePlayers();								//THIS HERE
	/*App->entityManager->SpawnEnemies();*/								//If SpawnEnemies is called here then it should not be called in the PreUpdate()

	cam_debug_speed = App->render->cam.camera_debug_speed;				//Sets the camera speed in debug mode.

	if (App->map->Load(map_names.start->data->GetString()) == true)		//If the first map is loaded then create the walkability map for it.
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))				//If CreatewalkabilityMap() returns true. It means that the walkability map could be created.
		{
			App->pathfinding->SetMap(w, h, data);						//Sets a new walkability map with the map passed by CreateWalkabilityMap().
		}

		RELEASE_ARRAY(data);											//Frees all memory allocated to the data array.
	}

	path_debug_tex = App->tex->Load("maps/path2_centered.png");					//Sets the path_debug_tex as path2.png.

	// UI
	// Upper bar
	App->gui->CreateImage({ 0,0 }, { 973,305,677,36 }, true);
	App->gui->CreateImage({ 0,36 }, { 973,305,677,36 }, true);
	App->gui->CreateImage({ 676,0 }, { 973,305,677,36 }, true);
	App->gui->CreateImage({ 676,36 }, { 973,305,677,36 }, true);
	App->gui->CreateImage({ 1352,0 }, { 973,305,377,36 }, true);
	App->gui->CreateImage({ 1352,36 }, { 973,305,377,36 }, true);

	// Character faces
	App->gui->CreateImage({ 70,20 }, { 1481,142,36,33 }, true);
	App->gui->CreateImage({ 935,20 }, { 1528,142,39,33 }, true);

	// Tiemr, score, etc
	App->gui->CreateLabel({ 10, 20 }, "Timm", Label_Type::FONT, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 980, 20 }, "Joe", Label_Type::FONT, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 225, 20 }, "Score", Label_Type::FONT, { 255,255,255,255 }, true);
	score_label = App->gui->CreateLabel({ 300,20 }, player_score_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 710, 20 }, "Timer", Label_Type::FONT, { 255,255,255,255 }, true);
	timer_label = App->gui->CreateLabel({ 785,20 }, timer_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 410, 20 }, "MUTUAL COOPERATION", Label_Type::FONT, { 255,255,255,255 }, true);

	// Lifes player 1 & player 2
	hearts[0] = (App->gui->CreateImage({ 110,30 }, { 1058,8,24,21 }, true));
	hearts[1] = (App->gui->CreateImage({ 135,30 }, { 1058,8,24,21 }, true));
	hearts[2] = (App->gui->CreateImage({ 160,30 }, { 1058,8,24,21 }, true));
	hearts[3] = (App->gui->CreateImage({ 910,30 }, { 1058,8,24,21 }, true));
	hearts[4] = (App->gui->CreateImage({ 885,30 }, { 1058,8,24,21 }, true));
	hearts[5] = (App->gui->CreateImage({ 860,30 }, { 1058,8,24,21 }, true));

	// Menu in-game
	pause_ui_list.add(App->gui->CreateImage({ 280,180 }, { 0, 388, 466, 447 }, true));
	pause_ui_list.add(App->gui->CreateImage({ 323, 170 }, { 1078,242,382,61 }, true));
	pause_ui_list.add(App->gui->CreateLabel({ 453,182 }, "PAUSE MENU", FONT, { 255,255,255,255 }, true));

	SDL_Rect button_rect[3] = { { 0,74,284,66 }, { 285,74,284,66 }, { 0,142,284,66 } };
	button_list.add(App->gui->CreateButton({ 370,245 }, Button_Type::PLAY, button_rect[0], &button_rect[1], &button_rect[2], "                   Resume", true));
	button_list.add(App->gui->CreateButton({ 370,315 }, Button_Type::SAVE, button_rect[0], &button_rect[1], &button_rect[2], "                      Save", true));
	button_list.add(App->gui->CreateButton({ 370,385 }, Button_Type::LOAD, button_rect[0], &button_rect[1], &button_rect[2], "                       Load", true));
	button_list.add(App->gui->CreateButton({ 370,455 }, Button_Type::EXIT, button_rect[0], &button_rect[1], &button_rect[2], "                       Exit", true));

	SDL_Rect mute_rect_button[3] = { { 1479, 9, 57, 57 }, { 986,6,57,57 }, { 294, 143, 57, 57 } };
	button_list.add(App->gui->CreateButton({ 670, 650 }, MUTE, mute_rect_button[0], &mute_rect_button[1], &mute_rect_button[2]));

	SDL_Rect unmute_rect_button[3] = { { 512,147,57,57 }, {342,98,57,57},{ 986,48,57,57 } };
	button_list.add(App->gui->CreateButton({ 300, 650 }, UNMUTE, unmute_rect_button[0], &unmute_rect_button[1], &unmute_rect_button[2]));

	p2List_item<UIitem_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		button_item->data->visible = false;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = pause_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = false;
		ui_item = ui_item->next;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	if (App->map->pathfindingMetaDebug == true)
	{
		static iPoint origin;
		static bool origin_selected = false;

		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (origin_selected == true)
			{
				App->pathfinding->CreatePath(origin, p);
				LOG("Tiles selected: (%d, %d) and (%d, %d)", origin.x, origin.y, p.x, p.y);
				origin_selected = false;
			}
			else
			{
				origin = p;
				origin_selected = true;
			}
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)														//Receives dt as an argument.
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::LavenderBlush);
	//Camera Movement With Arrow Keys
	if (App->render->cam.camera_debug == true)
	{
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
		App->fadescene->FadeToBlack("Test_map.tmx");
		
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)			//Load Second Level Key
	{
		App->fadescene->FadeToBlack("Test_map_2.tmx");
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)			//Restart Key
	{
		App->entityManager->player->Restart();
		App->entityManager->player2->Restart();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)			//Save Game Key
	{
		App->SaveGame("save_game.xml");
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)			//Load Game Key
	{
		App->LoadGame("save_game.xml");
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

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)			//PathfindingCollisions meta layer Debug Key
	{
		if (App->map->pathfindingMetaDebug == true)
		{
			App->map->pathfindingMetaDebug = false;
		}
		else
		{
			App->map->pathfindingMetaDebug = true;
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

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)		//God Mode Key
	{
		if (App->entityManager->player->player.GodMode)
		{
			App->entityManager->player->player.GodMode		= false;
			App->entityManager->player2->player.GodMode		= false;
			App->entityManager->player->player.airborne		= true;
			App->entityManager->player2->player.airborne	= true;
		}
		else
		{
			App->entityManager->player->player.GodMode		= true;
			App->entityManager->player2->player.GodMode		= true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)		//Enabling / Disabling frame cap
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

	if (App->map->pathfindingMetaDebug == true)
	{
		// Debug pathfinding ------------------------------
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);
		p = App->map->MapToWorld(p.x, p.y);

		App->render->Blit(path_debug_tex, p.x, p.y);								//Should we want it, we could make a separate texture called mouse_debug_tex so the tex at mouse pos and the tex at path tile are different.

		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(path_debug_tex, pos.x, pos.y);
		}
	}
	
	//Technical title
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d", App->map->data.width, App->map->data.height, App->map->data.tile_width, App->map->data.tile_height, App->map->data.tilesets.count());
	App->win->SetTitle(title.GetString());*/

	// UI
	float timer = (float)ptimer.ReadMs() / 1000;
	sprintf_s(timer_string, 20, "%.2f", timer);
	timer_label->ChangeText(timer_string);

	if (App->entityManager->player->player.lives == 3 || App->entityManager->player2->player.lives == 3)
	{
		hearts[0]->visible = true;
		hearts[1]->visible = true;
		hearts[2]->visible = true;
		hearts[3]->visible = true;
		hearts[4]->visible = true;
		hearts[5]->visible = true;
	}
	if (App->entityManager->player->player.lives == 2 || App->entityManager->player2->player.lives == 2)
	{
		hearts[0]->visible = true;
		hearts[1]->visible = true;
		hearts[2]->visible = false;
		hearts[3]->visible = true;
		hearts[4]->visible = true;
		hearts[5]->visible = false;
	}
	if (App->entityManager->player->player.lives == 1 || App->entityManager->player2->player.lives == 1)
	{
		hearts[0]->visible = true;
		hearts[1]->visible = false;
		hearts[2]->visible = false;
		hearts[3]->visible = true;
		hearts[4]->visible = false;
		hearts[5]->visible = false;
	}
	if (App->entityManager->player->player.lives == 0 || App->entityManager->player2->player.lives == 0)
	{
		hearts[0]->visible = true;
		hearts[1]->visible = true;
		hearts[2]->visible = true;
		hearts[3]->visible = true;
		hearts[4]->visible = true;
		hearts[5]->visible = true;
	}

	p2List_item<UIitem_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		if (button_item->data->OnClick())
		{
			switch (button_item->data->GetType())
			{
			case PLAY:
				ChangeVisibility();
				App->paused = false;
				break;
			case SAVE:
				App->SaveGame("save_game.xml");
				break;
			case LOAD:
				App->LoadGame("save_game.xml");
				break;
			case EXIT:
				App->fadescene->FadeToBlack2(App->scene, App->scene_menu);
				break;
			}
		}
		button_item = button_item->next;
	}

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

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		ChangeVisibility();
		App->paused = true;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	
	button_list.clear();
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

	App->gui->Disable();

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


void j1Scene::ChangeVisibility()
{
	p2List_item<UIitem_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		button_item->data->visible = !button_item->data->visible;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = pause_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;
	}
}
