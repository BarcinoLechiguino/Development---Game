#ifndef _UI_LABEL__H
#define _UI_LABEL__H

#include "p2Defs.h"
#include "UiItem.h"
#include "SDL/include/SDL.h"

enum Label_Type {
	TITLE,
	FONT,
	CONFIG,
	CREDITS,
	BUTTON
};

class UI_Label : public UI_Item
{
public:
	UI_Label(const char* text, Label_Type type, SDL_Color color, UI_Item* parent);
	~UI_Label();

	bool Start();
	bool PostUpdate();

	bool OnHover();
	void ChangeTexture(SDL_Color color);
	void ChangeText(const char* text);

private:
	Label_Type label_type;
	p2SString label_text;
	SDL_Color label_color;

public:

	mutable SDL_Texture* tex = nullptr;

};


#endif  // !_UI_LABEL__H
