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
#include "j1Audio.h"
#include "j1FadeScene.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1SceneMenu::j1SceneMenu() : j1Module()
{
	name.create("scene_menu");
}

// Destructor
j1SceneMenu::~j1SceneMenu()
{}

// Called before render is available
bool j1SceneMenu::Awake(pugi::xml_node& config)
{
	menu = true;
	settings = false;
	in_game = false;
	credits = false;
	play_music = true;

	music_path_menu.create(config.child("audio").attribute("path").as_string());

	return true;
}

// Called before the first frame
bool j1SceneMenu::Start()
{
	bool ret = true;

	// Menu
	// Background image
	image_ui_list.add(App->gui->CreateImage({ 45,0 }, { 1654,56,915,768 }, true));

	// Side bars
	menu_ui_list.add(App->gui->CreateImage({ 0,0 }, { 1601, 347, 45, 486 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 0,486 }, { 1601, 347, 45, 486 }, true));

	menu_ui_list.add(App->gui->CreateImage({ 960,0 }, { 1601, 347, 45, 486 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 1005,0 }, { 1601, 347, 45, 486 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 960,486 }, { 1601, 347, 45, 486 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 1005,486 }, { 1601, 347, 45, 486 }, true));

	// Titles
	menu_ui_list.add(App->gui->CreateImage({ 280,180 }, { 0, 388, 466, 447 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 323, 100 }, { 1078,242,382,61 }, true));
	menu_ui_list.add(App->gui->CreateLabel({ 338,113 }, "MUTUAL COOPERATION", MAIN_TITLE, { 255,255,255,255 }, true));
	menu_ui_list.add(App->gui->CreateImage({ 393, 145 }, { 973,342,237,39 }, true));
	menu_ui_list.add(App->gui->CreateLabel({ 398,155 }, "Can you trust your friend?", SUBTITLES, { 255,255,255,255 }, true));

	// Menu buttons
	SDL_Rect button_rect[3] = { { 1166,418,423,107 }, { 1166,556,423,107 }, { 1166,702,423,107 } };
	button_list.add(App->gui->CreateButton({ 305, 189 }, PLAY, button_rect[0], &button_rect[1], &button_rect[2], "", true));
	button_list.add(App->gui->CreateButton({ 305, 294 }, SETTINGS, button_rect[0], &button_rect[1], &button_rect[2], "", true));
	button_list.add(App->gui->CreateButton({ 305, 399 }, CONTINUE, button_rect[0], &button_rect[1], &button_rect[2], "", true));
	button_list.add(App->gui->CreateButton({ 305, 506 }, EXIT, button_rect[0], &button_rect[1], &button_rect[2], "", true));

	// Labels buttons
	menu_ui_list.add(App->gui->CreateLabel({ 465, 219 }, "PLAY", MAIN_TITLE_BUTTON, { 255,255,255,255 }, true));
	menu_ui_list.add(App->gui->CreateLabel({ 435, 329 }, "SETTINGS", MAIN_TITLE_BUTTON, { 255,255,255,255 }, true));
	menu_ui_list.add(App->gui->CreateLabel({ 435, 429 }, "CONTINUE", MAIN_TITLE_BUTTON, { 255,255,255,255 }, true));
	menu_ui_list.add(App->gui->CreateLabel({ 480, 535 }, "EXIT", MAIN_TITLE_BUTTON, { 255,255,255,255 }, true));

	//-------------------------------------------------------------------------------------------------------------------------

	// Settings
	// Settings menu
	sett_ui_list.add(App->gui->CreateImage({ 280,180 }, { 0, 388, 466, 447 }, true));
	sett_ui_list.add(App->gui->CreateImage({ 323, 160 }, { 1078,242,382,61 }, true));
	
	// Mute button
	SDL_Rect mute_rect_button[3] = { { 1479, 9, 57, 57 }, { 986,6,57,57 }, { 294, 143, 57, 57 } };
	button_list_sett.add(App->gui->CreateButton({ 300, 300 }, MUTE, mute_rect_button[0], &mute_rect_button[1], &mute_rect_button[2], "", true));
	
	// Unmute button
	SDL_Rect unmute_rect_button[3] = { { 512,147,57,57 }, {342,98,57,57},{ 986,48,57,57 } };
	button_list_sett.add(App->gui->CreateButton({ 670, 300 }, UNMUTE, unmute_rect_button[0], &unmute_rect_button[1], &unmute_rect_button[2], "", true));
	
	// Back button
	SDL_Rect button_back_rect[3] = { { 0,74,284,66 }, { 285,74,284,66 }, { 0,142,284,66 } };
	button_list_sett.add(App->gui->CreateButton({ 370,550 }, BACK, button_back_rect[0], &button_back_rect[1], &button_back_rect[2], "", true));

	//Capto30
	SDL_Rect capto30_rect_button[3] = { { 633,323,53,53 }, { 687,323,53,53 }, { 571,54,53,53 } };
	button_list_sett.add(App->gui->CreateButton({ 560, 419 }, CAPTO30, capto30_rect_button[0], &capto30_rect_button[1], &capto30_rect_button[2], "", true));

	// Credit button
	SDL_Rect credit_rect_button[3] = { { 744,320,58,58 }, {810,319,58,58},{ 880,319,58,58 } };
	button_list_sett.add(App->gui->CreateButton({ 300, 555 }, CREDIT, credit_rect_button[0], &credit_rect_button[1], &credit_rect_button[2], "", true));

	// Git button
	SDL_Rect git_rect_button[3] = { { 1066,178,58,58 }, {1066,178,58,58},{ 1066,178,58,58 } };
	button_list_sett.add(App->gui->CreateButton({ 670, 555 }, GITHUB, git_rect_button[0], &git_rect_button[1], &git_rect_button[2], "", true));

	// Text settings menu
	sett_ui_list.add(App->gui->CreateLabel({ 365, 173 }, "SETTINGS MENU", MAIN_TITLE_BUTTON, { 255,255,255,255 }, true));
	sett_ui_list.add(App->gui->CreateLabel({ 415, 253 }, "Sound Settings", TITLE_BUTTON, { 255,255,255,255 }, true));
	sett_ui_list.add(App->gui->CreateLabel({ 480, 573 }, "BACK", TITLE_BUTTON, { 255,255,255,255 }, true));
	sett_ui_list.add(App->gui->CreateLabel({ 300, 433 }, "Cap to 30 FPS", TITLE_BUTTON, { 255, 255, 255, 255 }, true));
	
	// Slider
	sett_ui_list.add(App->gui->CreateSlider({ 430,409 }, { 674,273,143,38 }, true));
	
	//-----------------------------------------------------------------------------------

	// Credits
	// Back
	credit_ui_list.add(App->gui->CreateImage({ 280,180 }, { 0, 388, 466, 447 }, true));
	credit_ui_list.add(App->gui->CreateImage({ 288,188 }, { 561, 383, 446, 427 }, true));
	
	// Titles
	credit_ui_list.add(App->gui->CreateImage({ 323, 160 }, { 1078,242,382,61 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 430, 173 }, "CREDITS", MAIN_TITLE_BUTTON, { 255,255,255,255 }, true));

	// Back Button
	SDL_Rect button_back_rect_cred[3] = { { 1057,30,173,45 }, { 1057,76,173,45 }, { 1057,126,173,45 } };
	button_list_credit.add(App->gui->CreateButton({ 430,570 }, BACK, button_back_rect_cred[0], &button_back_rect_cred[1], &button_back_rect_cred[2], "", true));
	credit_ui_list.add(App->gui->CreateLabel({ 490, 580 }, "BACK", TITLE_BUTTON, { 255,255,255,255 }, true));

	// Instructions
	credit_ui_list.add(App->gui->CreateLabel({ 300, 250 }, "MIT License Copyright(c)[2019][Gerard Romeu, Ángel González]", DEFAULT_BIGGER, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 300 }, "Permission is hereby granted, free of charge, to any person obtaining a copy", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 315 }, "of this software and associated documentation files(the Software), to deal in", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 330 }, "the Software without restriction, including without limitation the rights to use,", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 345 }, "copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 360 }, "Software, and to permit persons to whom the Software is furnished to do so, ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 375 }, "subject to the following conditions : ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 390 }, "The above copyright notice and this permission notice shall be included in all ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 405 }, "copies or substantial portions of the Software. ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 430 }, "THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 445 }, "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 460 }, "OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON  ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 475 }, "INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 490 }, " HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 505 }, "WHETHER IN AN  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 520 }, "FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR ", DEFAULT, { 255,255,255,255 }, true));
	credit_ui_list.add(App->gui->CreateLabel({ 298, 535 }, "OTHER DEALINGS IN THE SOFTWARE. ", DEFAULT, { 255,255,255,255 }, true));

	p2List_item<UIitem_Button*>* button_item = button_list_sett.start;
	while (button_item != NULL)
	{
		button_item->data->visible = false;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = sett_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = false;
		ui_item = ui_item->next;
	}
	p2List_item<UIitem_Button*>* button_item2 = button_list_credit.start;
	while (button_item2 != NULL)
	{
		button_item2->data->visible = false;
		button_item2 = button_item2->next;
	}
	p2List_item<UI_Item*>* ui_item2 = credit_ui_list.start;
	while (ui_item2 != NULL)
	{
		ui_item2->data->visible = false;
		ui_item2 = ui_item2->next;
	}

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

	if ((menu == true || settings == true || credits == true) &&( play_music == true))
	{
		App->audio->PlayMusic(music_path_menu.GetString());
		play_music = false;
	}

	if (settings)
	{
		p2List_item<UIitem_Button*>* button_item_sett = button_list_sett.start;
		while (button_item_sett != NULL)
		{
			if (button_item_sett->data->OnClick())
			{
				switch (button_item_sett->data->GetType())
				{
				case MUTE:
					App->audio->volume = 0;
					break;
				case UNMUTE:
					App->audio->volume = 100;
					break;
				case CAPTO30:
					if (App->framesAreCapped == true)
					{
						App->framesAreCapped = false;
					}
					else
					{
						App->framesAreCapped = true;
					}
					break;
				case CREDIT:
					ChangeVisibility_SETT();
					ChangeVisibility_CRED();
					credits = true;
					settings = false;
					break;
				case BACK:
					settings = false;
					menu = true;
					break;
				case GITHUB:
					ShellExecuteA(NULL, "open", "https://github.com/BarcinoLechiguino/Game---Development", NULL, NULL, SW_SHOWNORMAL);
				}

			}
			button_item_sett = button_item_sett->next;
		}

		
	}

	if (menu)
	{
		p2List_item<UIitem_Button*>* button_item = button_list.start;
		while (button_item != NULL)
		{
			if (button_item->data->OnClick())
			{
				switch (button_item->data->GetType())
				{
				case PLAY:
					ChangeVisibility_MENU();
					App->scene_ui->ChangeVisibility_HUD();
					ChangeVisibility_IMG();
					menu = false;
					no_music();
					App->audio->PlayMusic(App->scene->music_path.GetString());
					break;
				case SETTINGS:
					ChangeVisibility_MENU();
					ChangeVisibility_SETT();
					settings = true;
					menu = false;
					break;
				case CONTINUE:
					App->LoadGame("save_game.xml");
					ChangeVisibility_MENU();
					App->scene_ui->ChangeVisibility_HUD();
					menu = false;
					ChangeVisibility_IMG();
					no_music();
					App->audio->PlayMusic(App->scene->music_path.GetString());
					break;
				case EXIT:
					ret = false;
					break;
				}
			}

			button_item = button_item->next;
		}
	}

	if (credits)
	{
		p2List_item<UIitem_Button*>* button_item_cred = button_list_credit.start;
		while (button_item_cred != NULL)
		{
			if (button_item_cred->data->OnClick())
			{
				switch (button_item_cred->data->GetType())
				{
				case BACK:
					ChangeVisibility_CRED();
					ChangeVisibility_SETT();
					credits = false;
					settings = true;
					break;
				}
			}

			button_item_cred = button_item_cred->next;
		}
	}

	return ret;
}

// Called each loop iteration
bool j1SceneMenu::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneMenu", Profiler::Color::NavajoWhite);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}
		
	return ret;
}

bool j1SceneMenu::CleanUp()
{
	LOG("Freeing Scene Menu");

	button_list.clear();

	return true;
}

void j1SceneMenu::ChangeVisibility_MENU()
{
	p2List_item<UI_Item*>* ui_item = menu_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;

	}

	p2List_item<UIitem_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		button_item->data->visible = !button_item->data->visible;
		button_item = button_item->next;
	}
}

void j1SceneMenu::ChangeVisibility_SETT()
{
	p2List_item<UI_Item*>* ui_item = sett_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;

	}

	p2List_item<UIitem_Button*>* button_item = button_list_sett.start;
	while (button_item != NULL)
	{
		button_item->data->visible = !button_item->data->visible;
		button_item = button_item->next;
	}
}

void j1SceneMenu::ChangeVisibility_IMG()
{
	p2List_item<UI_Item*>* ui_item = image_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;

	}
}

void j1SceneMenu::ChangeVisibility_CRED()
{
	p2List_item<UI_Item*>* ui_item = credit_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;

	}

	p2List_item<UIitem_Button*>* button_item = button_list_credit.start;
	while (button_item != NULL)
	{
		button_item->data->visible = !button_item->data->visible;
		button_item = button_item->next;
	}
}

void j1SceneMenu::no_music()
{
	Mix_HaltMusic();
}