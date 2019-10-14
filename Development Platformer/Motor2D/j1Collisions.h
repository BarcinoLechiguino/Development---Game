#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"

enum class Collider_Type
{
	UKNOWN = -1,
	PLAYER,
	WALL,
	PLATFORM,
	HAZARD,
	SWITCH,
	WIN
};

struct Collider
{
	SDL_Rect collider;
	Collider_Type type;

	j1Module* callback = nullptr;

	bool delete_collider = false; //Used to delete colliders that are not needed anymore or

	Collider(SDL_Rect rectangle, Collider_Type type, j1Module* calback = nullptr) : collider(rectangle), type(type), callback(calback)//Should change?
	{

	};

	void SetPos(int x, int y)
	{
		collider.x = x;
		collider.y = y;
	}

	void SetSize(int w, int h)
	{
		collider.w = w;
		collider.h = h;
	}

	bool check_collision(const SDL_Rect& r) const;
};

class j1Collisions : public j1Module 
{
	//Methods
public:

	j1Collisions();

	bool Init();

	// Destructor
	virtual ~j1Collisions() {};

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();
	 
	// Called before quitting
	bool CleanUp();

private:
	//Variables
	p2List<Collider*> colliders;


public:

private:

};
#endif // __j1Collisions_H__