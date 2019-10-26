#ifndef  _MODULE_SCENECHANGE_
#define _MODULE_SCENECHANGE_

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

enum ListOfMapNames;

class j1Fade_Scene : public j1Module
{

public:

	j1Fade_Scene();

	virtual ~j1Fade_Scene();

	//Called before render is avalible
	bool Awake(pugi::xml_node&);

	//Called before the first frame
	bool Start();

	//Called each loop iteration
	bool Update(float dt);

	//Change map with a fade time
	bool ChangeMap(const char* newMap);

	//returns true if it's changing map
	bool IsChanging() const;

	//New, must change.
	bool FadeToBlack(const char* mapname, float time = 2.0f);

private:

public:

	bool fading = false;

private:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;


	int nextMap;
	uint start_time = 0;
	uint total_time = 0;
	SDL_Rect screen;
	const char* mapName;


};
#endif // ! _MODULE_SCENECHANGE_
