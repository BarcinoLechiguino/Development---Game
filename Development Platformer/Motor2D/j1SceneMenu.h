#ifndef _SCENE_MENU__H
#define _SCENE_MENU__H

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2List.h"
#include "j1Gui.h"
#include "UiItem_Button.h"
#include "UI_Image.h"
#include "UiItem_Label.h"

class j1SceneMenu : public j1Module
{
public:

	j1SceneMenu();

	// Destructor
	virtual ~j1SceneMenu();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	void ChangeVisibility_MENU(); 
	void ChangeVisibility_SETT();
	void ChangeVisibility_IMG();
	void ChangeVisibility_CRED();

public:
	void no_music();

public:

	p2List<UIitem_Button*>	button_list;
	p2List<UI_Item*>		menu_ui_list;
	p2List<UI_Item*>		image_ui_list;

	p2List<UIitem_Button*>	button_list_sett;
	p2List<UI_Item*>		sett_ui_list;

	p2List<UIitem_Button*>	button_list_credit;
	p2List<UI_Item*>		credit_ui_list;


	bool					menu = false;
	bool					settings = false;
	bool					in_game = false;
	bool					credits = false;

	bool					play_music = false;

	p2SString				music_path_menu;

	// HUD and in-game menu
public:
	void ChangeVisibility_ESC(); // When pause menu is displayed and closed

public:

	char		player_score_string[5] = "0";
	char		timer_string[20] = "0";
	uint        player_lifes = 3;
	UI_Item*     hearts[6] = { nullptr,nullptr,nullptr };
	uint		player_score = 0;
	UI_Label*	score_label = nullptr;
	UI_Label*	timer_label = nullptr;

	p2List<UIitem_Button*>	ingame_button_list;
	p2List<UI_Item*>		pause_ui_list; //Pause menu when esc pressed
	p2List<UI_Item*>		hud_list;

private:
	j1PerfTimer			ptimer;
};


#endif // !_SCENE_MENU__H