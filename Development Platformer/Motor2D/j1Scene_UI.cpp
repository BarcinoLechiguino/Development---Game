#include "j1App.h"
#include "j1Scene_UI.h"
#include "j1Scene.h"
#include "j1SceneMenu.h"
#include "p2Log.h"
#include "j1Input.h"
#include "UiItem.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Player.h"
#include "UiItem_Label.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1EntityManager.h"
#include "j1FadeScene.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1Scene_UI::j1Scene_UI() : j1Module()
{
	name.create("scene_UI");
}

// Destructor
j1Scene_UI::~j1Scene_UI()
{}

// Called before render is available
bool j1Scene_UI::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene UI");
	bool ret = true;
	path.create(config.child("path").child_value());
	return ret;
}

// Called before the first frame
bool j1Scene_UI::Start()
{
	bool ret = true;

	// Upper bar
	App->gui->CreateImage({ 0,0 }, { 649,203,451,24 }, true);
	App->gui->CreateImage({ 0,24 }, { 649,203,451,24 }, true);
	App->gui->CreateImage({ 451,0 }, { 649,203,451,24 }, true);
	App->gui->CreateImage({ 451,24 }, { 649,203,451,24 }, true);
	App->gui->CreateImage({ 902,0 }, { 649,203,251,24 }, true);
	App->gui->CreateImage({ 902,24 }, { 649,203,251,24 }, true);

	// Character faces
	App->gui->CreateImage({ 70,10 }, { 988,95,23,21 }, true);
	App->gui->CreateImage({ 950,10 }, { 1019,95,25,22 }, true);

	// Tiemr, score, etc
	App->gui->CreateLabel({ 10, 7 }, "Timm", Label_Type::FONT, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 980, 7 }, "Joe", Label_Type::FONT, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 225, 7 }, "Score", Label_Type::FONT, { 255,255,255,255 }, true);
	score_label = App->gui->CreateLabel({ 300,9 }, player_score_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 710, 7 }, "Timer", Label_Type::FONT, { 255,255,255,255 }, true);
	timer_label = App->gui->CreateLabel({ 785,5 }, timer_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 410, 7 }, "MUTUAL COOPERATION", Label_Type::FONT, { 255,255,255,255 }, true);

	// Lifes player 1 & player 2
	hearts[0] = (App->gui->CreateImage({110,15 }, { 706,6,14,13 }, true));
	hearts[1] = (App->gui->CreateImage({ 130,15 }, { 706,6,14,13 }, true));
	hearts[2] = (App->gui->CreateImage({ 150,15 }, { 706,6,14,13 }, true));
	hearts[3] = (App->gui->CreateImage({ 920,15 }, { 706,6,14,13 }, true));
	hearts[4] = (App->gui->CreateImage({ 900,15 }, { 706,6,14,13 }, true));
	hearts[5] = (App->gui->CreateImage({ 880,15 }, { 706,6,14,13 }, true));

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

	return ret;
}

// Called each loop iteration
bool j1Scene_UI::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene_UI::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneUI", Profiler::Color::DarkKhaki);
	float timer = (float)ptimer.ReadMs() / 1000;
	sprintf_s(timer_string, 20, "%.2f", timer);
	timer_label->ChangeText(timer_string);

	// if condiotioning with lifes here

	/*App->entityManager->player->player.lives;*/

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
bool j1Scene_UI::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneIntro", Profiler::Color::MediumVioletRed);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ChangeVisibility();
	}

	return ret;
}

// Called before quitting
bool j1Scene_UI::CleanUp()
{
	LOG("Freeing Scene Menu");
	button_list.clear();
	App->gui->Disable();
	return true;
}

void j1Scene_UI::ChangeVisibility()
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

