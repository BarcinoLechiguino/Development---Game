#ifndef __ModuleMecha__
#define __ModuleMecha__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class Mecha : public j1Entity
{
public:
	Mecha(int x, int y, ENTITY_TYPE type);
	~Mecha();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Normal_Path();
	void Chasing_Path();
	void AddAnimationPushbacks();

public:

	SDL_Texture* graphics = nullptr;

};

#endif // !__ModuleMecha__
