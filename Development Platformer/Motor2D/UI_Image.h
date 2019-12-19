#ifndef _UI_SPRITE__H
#define _UI_SPRITE__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

class UI_Image : public UI_Item
{
public:
	UI_Image(SDL_Rect rect, UI_Item* parent);
	~UI_Image() {};

	bool PostUpdate();

private:
	SDL_Rect sprite_rect;

};




#endif  // !_UI_SPRITE__H