#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2List.h"
#include "j1Gui.h"
#include "UI_Image.h"

struct SDL_Texture;

class UI;
class UI_Image;
class UI_Text;
class UI_Button;
class UI_InputBox;
class UI_Scrollbar;

enum ListOfMapNames
{
	TutorialLevel = 0,
	FirstLevel
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

public:
	bool Load_lvl(int time);

	void LoadGuiElements();


public:
	int					currentMap;
	p2List<p2SString*>	map_names;
	float				fade_time;
	bool				to_end;
	p2SString			music_path;
	bool				firstMap;			//Keeps track of which map is loaded. In this case this one keeps track of the first map.
	bool				secondMap;			//Keeps track of which map is loaded. In this case this one keeps track of the second map.

	// Testing the UI
	UI_Image* banner;
	UI_Text* text;
	
	UI_Image*		window;
	UI_Text*		interactibleText;
	UI_Text*		buttonLabel;
	UI_Button*		button;
	UI_Button*		escButton;
	UI_Button*		draggableButton;
	UI_Button*		draggableButton2;

	UI_InputBox*	inputBox;

	UI_Image*		scrollWindow;
	UI_Text*		scrollText;
	UI_InputBox*	scrollInputBox;
	UI_Scrollbar*	scrollbar;
	UI_Image*		thumbo;

private:
	float				cam_debug_speed;	//Will store the movement speed for the camera while in debug mode. Done for readability.
	//SDL_Texture*		mouse_debug_tex;	//Texture that will appear at mouse position when pathfinding is being debugged.
	SDL_Texture*		path_debug_tex;		//Texture that will appear at the path tiles when pathfinding is being debugged.

	// UI VARIABLES
};

#endif // __j1SCENE_H__