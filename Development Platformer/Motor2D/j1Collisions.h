#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"

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