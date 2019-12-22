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
	hearts[0] = (App->gui->CreateImage({110,30 }, { 1058,8,24,21 }, true));
	hearts[1] = (App->gui->CreateImage({ 135,30 }, { 1058,8,24,21 }, true));
	hearts[2] = (App->gui->CreateImage({ 160,30 }, { 1058,8,24,21 }, true));
	hearts[3] = (App->gui->CreateImage({ 910,30 }, { 1058,8,24,21 }, true));
	hearts[4] = (App->gui->CreateImage({ 885,30 }, { 1058,8,24,21 }, true));
	hearts[5] = (App->gui->CreateImage({ 860,30 }, { 1058,8,24,21 }, true));

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

