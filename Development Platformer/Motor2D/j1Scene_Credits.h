#ifndef _SCENE_CREDITS__H
#define _SCENE_CREDITS_H

#include "j1Module.h"
#include "p2DynArray.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2List.h"
#include "j1Gui.h"
#include "UiItem_Label.h"


class j1Scene_Credits : public j1Module
{
public:

	j1Scene_Credits();

	// Destructor
	virtual ~j1Scene_Credits();

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



private:
	p2SString			  path;
	UI_Label* credits_lbl = nullptr;

	p2SString             credits;
	p2DynArray<UI_Label*> labels;
	int scrolling = 1;
	bool over = false;


public:
	p2List<UIitem_Button*>	button_list;
};

#endif // !_SCENE_PAUSE__H