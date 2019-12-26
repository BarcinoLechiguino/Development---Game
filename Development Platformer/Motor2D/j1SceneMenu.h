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

public:

	p2List<UIitem_Button*>	button_list;
	p2List<UI_Item*>		menu_ui_list;
};


#endif // !_SCENE_MENU__H