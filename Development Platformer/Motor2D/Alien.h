#ifndef __ModuleMecha__
#define __ModuleMecha__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class Mecha : public j1Entity
{
public:
	Mecha();
	~Mecha();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void followPath();
	void standardPath();

public:
	
};

#endif // !__ModuleMecha__
