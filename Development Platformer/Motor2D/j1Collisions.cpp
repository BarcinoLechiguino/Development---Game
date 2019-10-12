#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"


j1Collisions::j1Collisions() : j1Module()
{
	name.create("collisions");
}

bool j1Collisions::Init() {

	return true;
};

bool j1Collisions::Awake(pugi::xml_node& config) {


	return true;
};

bool j1Collisions::Start() {




	return true;
};

bool j1Collisions::PreUpdate() {



	return true;
};

bool j1Collisions::Update(float dt) {


	return true;
};

bool j1Collisions::PostUpdate() {

	return true;
};

bool j1Collisions::CleanUp() {

	return true;
};

bool Collider::CheckCollision(const SDL_Rect& r) const
{

	return ((r.x + r.w > rect.x) && (r.x < rect.x + rect.w) &&
		(r.y + r.h > rect.y) && (r.y < rect.y + rect.h));

}
