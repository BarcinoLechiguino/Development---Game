#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

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

	bool Load_lvl(int time);

public:
	int					currentMap;
	p2List<p2SString*>	map_names;
	float				fade_time;
	bool				to_end;

	bool				firstMap;			//Keeps track of which map is loaded. In this case this one keeps track of the first map.
	bool				secondMap;			//Keeps track of which map is loaded. In this case this one keeps track of the second map.

private:
	float				cam_debug_speed;	//Will store the movement speed for the camera while in debug mode. Done for readability.
	//SDL_Texture*		mouse_debug_tex;	//Texture that will appear at mouse position when pathfinding is being debugged.
	SDL_Texture*		path_debug_tex;		//Texture that will appear at the path tiles when pathfinding is being debugged.

};

#endif // __j1SCENE_H__