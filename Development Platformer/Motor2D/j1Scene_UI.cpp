#include "j1App.h"
#include "j1Scene_UI.h"
#include "j1Scene.h"
#include "j1SceneMenu.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Player1.h"
#include "j1Player2.h"
#include "j1Input.h"
#include "UiItem.h"
#include "j1Render.h"
#include "UiItem_Label.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1EntityManager.h"
#include "j1fadescene.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1Scene_UI::j1Scene_UI() : j1Module()
{

}

// Destructor
j1Scene_UI::~j1Scene_UI()
{}

// Called before render is available
bool j1Scene_UI::Awake(pugi::xml_node& config)
{
	return true;
}

// Called before the first frame
bool j1Scene_UI::Start()
{
	// HUD
	// Upper bar
	hud_list.add(App->gui->CreateImage({ 0,0 }, { 973,305,677,36 }, true));
	hud_list.add(App->gui->CreateImage({ 0,36 }, { 973,305,677,36 }, true));
	hud_list.add(App->gui->CreateImage({ 676,0 }, { 973,305,677,36 }, true));
	hud_list.add(App->gui->CreateImage({ 676,36 }, { 973,305,677,36 }, true));
	hud_list.add(App->gui->CreateImage({ 1352,0 }, { 973,305,377,36 }, true));
	hud_list.add(App->gui->CreateImage({ 1352,36 }, { 973,305,377,36 }, true));

	// Character faces
	hud_list.add(App->gui->CreateImage({ 70,20 }, { 1481,142,36,33 }, true));
	hud_list.add(App->gui->CreateImage({ 935,20 }, { 1528,142,39,33 }, true));

	// Tiemr, score, etc
	hud_list.add(App->gui->CreateLabel({ 10,30 }, "Timm", Label_Type::HUD, { 255,255,255,255 }, true));
	hud_list.add(App->gui->CreateLabel({ 980, 30 }, "Joe", Label_Type::HUD, { 255,255,255,255 }, true));
	hud_list.add(App->gui->CreateLabel({ 210, 30 }, "Score", Label_Type::HUD, { 255,255,255,255 }, true));
	score_label = App->gui->CreateLabel({ 285,30 }, player_score_string, Label_Type::HUD, { 255,255,255,255 }, true);
	hud_list.add(App->gui->CreateLabel({ 695, 30 }, "Timer", Label_Type::HUD, { 255,255,255,255 }, true));
	timer_label = App->gui->CreateLabel({ 770,30 }, timer_string, Label_Type::HUD, { 255,255,255,255 }, true);
	hud_list.add(App->gui->CreateLabel({ 375, 30 }, "MUTUAL COOPERATION", Label_Type::HUD, { 255,255,255,255 }, true));

	// Lifes player 1 & player 2
	hearts[0] = (App->gui->CreateImage({ 110,30 }, { 1058,8,24,21 }, true));
	hearts[1] = (App->gui->CreateImage({ 135,30 }, { 1058,8,24,21 }, true));
	hearts[2] = (App->gui->CreateImage({ 160,30 }, { 1058,8,24,21 }, true));
	hearts[3] = (App->gui->CreateImage({ 910,30 }, { 1058,8,24,21 }, true));
	hearts[4] = (App->gui->CreateImage({ 885,30 }, { 1058,8,24,21 }, true));
	hearts[5] = (App->gui->CreateImage({ 860,30 }, { 1058,8,24,21 }, true));

	// -------------------------------------------------------------------------------------------------------------------------------

	// In-game menu
	// Menu in-game
	pause_ui_list.add(App->gui->CreateImage({ 280,180 }, { 0, 388, 466, 447 }, true));
	pause_ui_list.add(App->gui->CreateImage({ 323, 170 }, { 1078,242,382,61 }, true));
	pause_ui_list.add(App->gui->CreateLabel({ 440,182 }, "PAUSE MENU", OTHER_TITLES, { 255,255,255,255 }, true));

	// Buttons menu in-game
	SDL_Rect button_rect[3] = { { 0,74,284,66 }, { 285,74,284,66 }, { 0,142,284,66 } };
	button_list.add(App->gui->CreateButton({ 370,247 }, Button_Type::PLAY, button_rect[0], &button_rect[1], &button_rect[2], "", true));
	button_list.add(App->gui->CreateButton({ 370,317 }, Button_Type::SAVE, button_rect[0], &button_rect[1], &button_rect[2], "", true));
	button_list.add(App->gui->CreateButton({ 370,387 }, Button_Type::LOAD, button_rect[0], &button_rect[1], &button_rect[2], "", true));
	button_list.add(App->gui->CreateButton({ 370,457 }, Button_Type::EXIT, button_rect[0], &button_rect[1], &button_rect[2], "", true));

	// Mute button
	SDL_Rect mute_rect_button[3] = { { 1479, 9, 57, 57 }, { 986,6,57,57 }, { 294, 143, 57, 57 } };
	button_list.add(App->gui->CreateButton({ 300, 550 }, MUTE, mute_rect_button[0], &mute_rect_button[1], &mute_rect_button[2], "", true));
	
	// Unmute button
	SDL_Rect unmute_rect_button[3] = { { 512,147,57,57 }, {342,98,57,57},{ 986,48,57,57 } };
	button_list.add(App->gui->CreateButton({ 670, 550 }, UNMUTE, unmute_rect_button[0], &unmute_rect_button[1], &unmute_rect_button[2], "", true));

	// Labels in-game menu
	pause_ui_list.add(App->gui->CreateLabel({ 465,267 }, "RESUME", TITLE_BUTTON, { 255,255,255,255 }, true));
	pause_ui_list.add(App->gui->CreateLabel({ 480,337 }, "SAVE", TITLE_BUTTON, { 255,255,255,255 }, true));
	pause_ui_list.add(App->gui->CreateLabel({ 480,407 }, "LOAD", TITLE_BUTTON, { 255,255,255,255 }, true));
	pause_ui_list.add(App->gui->CreateLabel({ 480,477 }, "EXIT", TITLE_BUTTON, { 255,255,255,255 }, true));
	
	// Slider
	pause_ui_list.add(App->gui->CreateSlider({ 440,659 }, { 674,273,143,38 }, true));

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
	p2List_item<UI_Item*>* ui_item2 = hud_list.start;
	while (ui_item2 != NULL)
	{
		ui_item2->data->visible = false;
		ui_item2 = ui_item2->next;
	}

	return true;
}

// Called each loop iteration
bool j1Scene_UI::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_SceneUI", Profiler::Color::CornflowerBlue);

	return true;
}

// Called each loop iteration
bool j1Scene_UI::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneUI", Profiler::Color::CornflowerBlue);

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

	if (App->scene_menu->in_game)
	{
		p2List_item<UIitem_Button*>* button_item = button_list.start;
		while (button_item != NULL)
		{
			if (button_item->data->OnClick())
			{
				switch (button_item->data->GetType())
				{
				case PLAY:
					ChangeVisibility_ESC();
					App->scene_menu->in_game = false;
					break;
				case SAVE:
					App->SaveGame("save_game.xml");
					break;
				case LOAD:
					App->LoadGame("save_game.xml");
					ChangeVisibility_ESC();
					App->scene_menu->in_game = false;
					break;
				case EXIT:
					ChangeVisibility_HUD();
					ChangeVisibility_ESC();
					App->scene_menu->ChangeVisibility_MENU();
					App->scene_menu->in_game = false;
					App->scene_menu->menu = true;
					App->scene_menu->ChangeVisibility_IMG();
					break;
				case MUTE:
					App->audio->volume = 0;
					break;
				case UNMUTE:
					App->audio->volume = 100;
					break;
				}
			}
			button_item = button_item->next;
		}
	}
	return true;
}

// Called each loop iteration
bool j1Scene_UI::PostUpdate()
{
	if (App->scene_menu->menu == false)
	{
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && App->scene_menu->in_game == false)
		{
			ChangeVisibility_ESC();
			App->scene_menu->in_game = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && App->scene_menu->in_game == true)
		{
			ChangeVisibility_ESC();
			App->scene_menu->in_game = false;
		}
	}
	return true;

}

// Called before quitting
bool j1Scene_UI::CleanUp()
{
	LOG("Freeing scene");

	button_list.clear();

	return true;
}

void j1Scene_UI::ChangeVisibility_ESC()
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

void j1Scene_UI::ChangeVisibility_HUD()
{

	p2List_item<UI_Item*>* ui_item = hud_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;

	}
}

