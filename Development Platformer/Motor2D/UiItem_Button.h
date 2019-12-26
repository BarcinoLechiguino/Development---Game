#ifndef _UI_BUTTON__H
#define _UI_BUTTON__H

#include "p2Defs.h"
#include "UiItem.h"
#include "SDL/include/SDL.h"

enum Button_Type //Types of buttons
{
	PLAY,
	SETTINGS,
	CONTINUE,
	CREDIT,
	MUTE,
	UNMUTE,
	BACK,
	CAPTO30,
	SAVE,
	LOAD,
	EXIT
};

class  UIitem_Button :public UI_Item
{
public:
	UIitem_Button(const char* text, Button_Type type, SDL_Rect idle_rect, SDL_Rect* rect_hover, SDL_Rect* rect_click, UI_Item* parent);
	~UIitem_Button();

	bool Start();
	bool PostUpdate();
	Button_Type	GetType();

	bool OnHover(); // To know the if the button is OnHoverState


private:
	SDL_Rect	button_rect[MAX_STATE]; //Rect of the button
	p2SString	button_text; // Text inside the button
	Button_Type	button_type; // Type of the button
};

#endif  // !_UI_BUTTON__H