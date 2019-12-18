#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "UiItem_Button.h"
#include "j1Render.h"
#include "UiItem_Label.h"
#include "UiItem_Bar.h"
#include "j1Map.h"
#include "j1Fonts.h"
#include "j1Scene.h"
//#include "j1Scene_Credits.h"
#include "j1Audio.h"
#include "j1FadeScene.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

//j1SceneMenu::j1SceneMenu() : j1Module()
//{
//	name.create("scene_menu");
//}
//
//// Destructor
//j1SceneMenu::~j1SceneMenu()
//{}
//
//// Called before render is available
//bool j1SceneMenu::Awake(pugi::xml_node& config)
//{
//	LOG("Loading Scene Intro");
//	bool ret = true;
//	path.create(config.child("path").child_value());
//	map_path.create(config.child("map").attribute("path").as_string());
//	music_path.create(config.child("audio").attribute("path").as_string());
//	return ret;
//}