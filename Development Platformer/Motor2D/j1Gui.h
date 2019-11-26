#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"


class Animation;

struct Collider;
struct SDL_Texture;

enum class ENTITY_TYPE
{
	UNKNOWN_TYPE,
	PLAYER,
	PLAYER2,
	MECHA,
	ALIEN,
};

struct EntityData
{
	ENTITY_TYPE		type;
	iPoint			position;
};

class j1Gui : public j1Module
{
public:
	j1Gui(int x, int y, ENTITY_TYPE type); // here we will set initial position

	//Must be virtual so when an entity is executed the compiler goes to the entity's methods instead of these
	virtual bool Awake(pugi::xml_node&);

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update(float dt, bool doLogic);

	virtual bool PostUpdate();

	virtual bool CleanUp();

	const SDL_Texture* GetAtlas() const;

public:

	SDL_Texture*	atlas;				//Sprite / Spritesheet of the entity.
	p2SString		atlas_file_name;	
};

#endif // __j1GUI_H__