#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"

enum Object_Type;
struct ObjectData;

struct Collider
{
	SDL_Rect collider;
	Object_Type type;
	j1Module* callback = nullptr;
	bool delete_collider = false; //Used to delete colliders that are not needed anymore or

	//Revise all this.
	Collider(SDL_Rect collider, Object_Type type, j1Module* callback = nullptr) : collider(collider), type(type), callback(callback)//Should change?
	{

	};

	Collider(ObjectData object);

	Collider() {};

	void Set_Position(int x, int y)
	{
		collider.x = x;
		collider.y = y;
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

	Collider* AddCollider(SDL_Rect collider, Object_Type type, j1Module* callback);
	void LoadFromMap();

private:
	void Collider_Debug();

public:
	//Variables
	Collider collider;
	p2List<Collider*> collider_list;
	bool collider_debug;

private:

};
#endif // __j1Collisions_H__