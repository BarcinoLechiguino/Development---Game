#ifndef UIITEM_LABEL_H
#define UIITEM_LABEL_H

#include "UiItem.h"
#include "p2SString.h"

#include "SDL_ttf/include/SDL_ttf.h"  //SDL that allows workikng with fonts
#include "p2Point.h"

class UiItem_Label : public UiItem
{

private:

	TTF_Font* font = nullptr; // Font
	p2SString text; // Text that will have the font
	SDL_Color color; // Colour of the text

	SDL_Texture* texture = nullptr; // Normal texture
	SDL_Texture* textureHover = nullptr; // Texture when hover state

public:

	UiItem_Label(p2SString text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem * const parent); 
	bool ChangeTextureHover(const p2SString * textHover, const SDL_Color* color, const TTF_Font* font); // Change texture to Hover texture
	bool ChangeTextureIdle(const p2SString * textHover, const SDL_Color* color, const TTF_Font* font); // Change texture to idle back again
	void Draw(const float& dt) override; // Draw labels on screen
};

#endif // !UIITEM_LABEL_H