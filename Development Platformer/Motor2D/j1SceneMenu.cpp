#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "UiItem_Button.h"
#include "j1Render.h"
#include "j1Scene_UI.h"
#include "UiItem_Label.h"
#include "UiItem_Bar.h"
#include "j1Map.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1Scene_Credits.h"
#include "j1Audio.h"
#include "j1FadeScene.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1SceneMenu::j1SceneMenu() : j1Module()
{
}

// Destructor
j1SceneMenu::~j1SceneMenu()
{}

// Called before render is available
bool j1SceneMenu::Awake(pugi::xml_node& config)
{
	return true;
}

// Called before the first frame
bool j1SceneMenu::Start()
{
	bool ret = true;

	// Background image
	menu_ui_list.add(App->gui->CreateImage({ 45,0 }, { 1654,56,915,768 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 280,180 }, { 0, 388, 466, 447 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 323, 100 }, { 1078,242,382,61 }, true));
	menu_ui_list.add(App->gui->CreateLabel({ 395,110 }, "MUTUAL COOPERATION", MAIN_TITLE, { 255,255,255,255 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 393, 145 }, { 973,342,237,39 }, true));
	menu_ui_list.add(App->gui->CreateLabel({ 395,140 }, "Can you trust your friend?", MAIN_TITLE, { 255,255,255,255 }, true));

	// Menu buttons
	SDL_Rect button_rect[3] = { { 1166,418,423,107 }, { 1166,556,423,107 }, { 1166,702,423,107 } };
	button_list.add(App->gui->CreateButton({ 305, 289 }, PLAY, button_rect[0], &button_rect[1], &button_rect[2], "                         Play"));
	button_list.add(App->gui->CreateButton({ 305, 394 }, SETTINGS, button_rect[0], &button_rect[1], &button_rect[2], "                         Settings"));
	button_list.add(App->gui->CreateButton({ 305, 499 }, CONTINUE, button_rect[0], &button_rect[1], &button_rect[2], "                         Continue"));
	button_list.add(App->gui->CreateButton({ 305, 606 }, EXIT, button_rect[0], &button_rect[1], &button_rect[2], "                          Exit"));

	// Credit button
	SDL_Rect credit_rect_button[3] = { { 744,320,58,58 }, {496,213,58,58},{ 541,213,58,58 } };
	button_list.add(App->gui->CreateButton({ 130, 713 }, CREDIT, credit_rect_button[0], &credit_rect_button[1], &credit_rect_button[2]));
	return ret;
}

// Called each loop iteration
bool j1SceneMenu::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool j1SceneMenu::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneMenu", Profiler::Color::NavajoWhite);
	bool ret = true;

	return ret;
}

// Called each loop iteration
bool j1SceneMenu::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneMenu", Profiler::Color::NavajoWhite);

	bool ret = true;

	return ret;
}

bool j1SceneMenu::CleanUp()
{
	LOG("Freeing Scene Menu");

	return true;
}

void j1SceneMenu::ChangeVisibility_MENU()
{

}