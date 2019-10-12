#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"

enum class Collider_Type;

struct Collider
{

	SDL_Rect rect;
	bool to_delete = false;
	Collider_Type type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, Collider_Type type, j1Module* calback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{};

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	void SetSize(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}
	bool CheckCollision(const SDL_Rect& r) const;

};

class j1Collisions : public j1Module {
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

public:

private:

};
#endif // __j1Collisions_H__