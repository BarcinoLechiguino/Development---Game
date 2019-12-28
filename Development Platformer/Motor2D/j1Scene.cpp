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
#include "j1SceneMenu.h"
#include "j1Collisions.h"
#include "j1Gui.h"
#include "j1EntityManager.h"
#include "j1Player1.h"
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

	music_path.create(config.child("audio").attribute("path").as_string());

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	to_end = false;
	
	firstMap	= true;
	secondMap	= false;

	ret = App->map->Load(map_names.start->data->GetString());
	LOG("Map Name: %s", map_names.start->data->GetString());

	App->entityManager->CreatePlayers();								//THIS HERE
	/*App->entityManager->SpawnEnemies();*/								//If SpawnEnemies is called here then it should not be called in the PreUpdate()

	cam_debug_speed = App->render->cam.camera_debug_speed;				//Sets the camera speed in debug mode.

	if (ret == true)													//If the first map is loaded then create the walkability map for it.
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

	LoadGuiElements();

	return ret;

	// UI

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

	App->gui->CleanUp();
	//App->gui->Disable();

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

void j1Scene::LoadGuiElements()
{
	// ------------------------------------------- UI TEST -------------------------------------------
	// TODO 3: Create the banner (rect {485, 829, 328, 103}) as a UI element
	SDL_Rect rect{ 485, 829, 328, 103 };
	SDL_Rect winRect{ 0, 512, 483, 512 };

	window = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, 225, 25, winRect, true, false, true, NULL);		//OnCallEvent Overlap
	banner = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, 300, 100, rect, true, false, false, window);

	// TODO 4: Create the text "Hello World" as a UI element
	SDL_Rect textHitbox{ 432, 75, 65, 20 };
	SDL_Rect interactibleTextHitbox{ 432, 75, 134, 20 };
	p2SString bufferString = "Hello World";
	p2SString interactibleString = "This string is interactible.";
	_TTF_Font* font = App->font->Load("fonts/open_sans/OpenSans-SemiboldItalic.ttf");
	SDL_Color fontRgb = { 255, 255, 255, 255 };

	interactibleText = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 280, 75, interactibleTextHitbox, font, fontRgb, true, true, false, window, &interactibleString);
	text = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 432, 75, textHitbox, font, fontRgb, true, false, false, window, &bufferString);


	//Creating a button:
	SDL_Rect buttonHitbox = { 642,169,229,69 };
	SDL_Rect idle = { 642,169,229,69 };
	SDL_Rect hover = { 0,113,229,69 };
	SDL_Rect clicked = { 411,169,229,69 };

	SDL_Rect buttonLabelHitbox{ 432, 75, 45, 18 };
	p2SString buttonString = "BUTTON";

	button = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 205, true, true, false, window, &idle, &hover, &clicked);
	escButton = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 280, true, true, false, window, &idle, &hover, &clicked);

	draggableButton = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 425, true, true, true, window, &idle, &hover, &clicked);
	buttonLabel = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 442, 450, buttonLabelHitbox, font, fontRgb, true, false, false, draggableButton, &buttonString);

	//draggableButton2 = (UI_Button*)App->gui->CreateButton(UI_Element::BUTTON, 350, 425, true, true, true, window, &idle, &hover, &clicked);

	//Creating a text box:
	SDL_Rect inputBoxBg = { 488, 569, 344, 61 };
	SDL_Rect inputBoxTxt = { 0, 0, 325, 30 };
	SDL_Rect textCursor = { 0, 0, 3, 40 };

	SDL_Color fontColour = { 255, 255, 255, 255 };
	SDL_Color cursorColour = { 255, 255, 255, 255 };
	float blinkFrequency = 0.5f;

	p2SString defaultTxt = "Input Text Here";
	_TTF_Font* inputFont = App->font->Load("fonts/open_sans/OpenSans-SemiboldItalic.ttf", 30);
	iPoint textOffset = { 10, 9 };

	inputBox = (UI_InputBox*)App->gui->CreateInputBox(UI_Element::INPUTBOX, 295, 355, inputBoxBg, inputFont, fontColour, textCursor, cursorColour, textOffset,
		blinkFrequency, true, true, false, window, &defaultTxt);

	//Creating a Scrollbar:
	SDL_Rect scrollbarBar = { 973, 786, 11, 158 };
	//SDL_Rect scrollbarThumb		= { 843, 330, 15, 10 };
	SDL_Rect scrollbarThumb = { 1004, 439, 15, 10 };
	iPoint thumbOffset = { -2, 0 };
	SDL_Rect scrollMask = { 0, 0, 350, 158 };
	iPoint maskOffset = { -360, 0 };
	SDL_Rect dragArea = { 0, 0, 11, 158 };
	float dragFactor = 0.2f;

	char* txt = "Lorem ipsum dolor sit amet,\n consectetur adipiscing elit.\n Integer blandit arcu turpis,\n vitae blandit lacus \n malesuada commodo.";
	p2SString scrollTxt = "Lorem ipsum dolor sit amet";
	_TTF_Font* scrollFont = App->font->Load("fonts/open_sans/OpenSans-Regular.ttf", 24);
	SDL_Color scrollFontColour = { 255, 255, 255, 255 };

	scrollWindow = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, 225, 25, winRect, false, false, false, NULL);
	scrollText = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 290, 325, textHitbox, scrollFont, scrollFontColour, false, false, false, scrollWindow, &scrollTxt);
	scrollInputBox = (UI_InputBox*)App->gui->CreateInputBox(UI_Element::INPUTBOX, 295, 155, inputBoxBg, inputFont, fontColour, textCursor, cursorColour, textOffset,
		blinkFrequency, false, true, false, scrollWindow, &defaultTxt);

	scrollbar = (UI_Scrollbar*)App->gui->CreateScrollbar(UI_Element::SCROLLBAR, 650, 325, scrollbarBar, scrollbarThumb, thumbOffset, dragArea, dragFactor, false, true, true,
		false, false, false, scrollWindow, &scrollMask, maskOffset);

	scrollbar->LinkScroll(scrollText);
	//thumbo = (UI_Image*)App->gui->CreateImage(UI_Element::IMAGE, 650 + thumbOffset.x, 300 + thumbOffset.y, scrollbarThumb, true, true, true, NULL);
}

