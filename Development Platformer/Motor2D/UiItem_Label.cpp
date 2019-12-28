#include "UiItem_Label.h"
#include "UiItem.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Defs.h"
#include "Brofiler\Brofiler.h"

UI_Label::UI_Label(const char* text, Label_Type type, SDL_Color color, UI_Item* parent)
{
	label_text = text;
	label_type = type;
	label_color = color;

	this->parent = parent;
}

UI_Label::~UI_Label()
{
	if (tex != nullptr)
	{
		App->tex->UnLoad(tex);
		tex = nullptr;
	}
}

bool UI_Label::Start()
{
	ChangeTexture(label_color);
	return true;
}

bool UI_Label::PostUpdate()
{
	App->render->Blit(tex, position.x, position.y);

	switch (state)
	{
	case IDLE:
		ChangeTexture({ 0,0,0,0 });
		break;
	case HOVER:
		if (label_type != MAIN_TITLE && label_type != OTHER_TITLES && label_type != HUD && label_type != SUBTITLES) {
			ChangeTexture({ 230,214,144,255 });
		}
		break;

	case CLICK:
		break;
	}

	return true;
}

bool UI_Label::OnHover()
{
	uint w, h;
	App->tex->GetSize(tex, w, h);
	bool ret = false;
	if (parent != nullptr)
	{
		ret = parent->OnHover();
	}
	return ((position.x < mouse_position.x && position.y < mouse_position.y && position.x + (int)w > mouse_position.x && position.y + (int)h > mouse_position.y) || ret);
}

void UI_Label::ChangeTexture(SDL_Color color)
{
	if (tex != nullptr)
	{
		App->tex->UnLoad(tex);
		tex = nullptr;
	}


	label_color = color;

	switch (label_type) {
	case MAIN_TITLE:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->main_title);
		break;
	case TITLE_BUTTON:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->title_buttons);
		break;
	case HUD:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->hud);
		break;
	case OTHER_TITLES:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->titles_other);
		break;
	case SUBTITLES:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->subtitle);
		break;
	case MAIN_TITLE_BUTTON:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->main_title_buttons);
		break;
	case DEFAULT:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->default);
		break;
	case DEFAULT_BIGGER:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->default_bigger);
		break;
	}
}

void UI_Label::ChangeText(const char * text)
{
	if (tex != nullptr)
	{
		App->tex->UnLoad(tex);
		tex = nullptr;
	}

	//label_text.Clear();
	//label_text.create(text);														

	switch (label_type) {
	case MAIN_TITLE:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->main_title);
		break;
	case TITLE_BUTTON:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->title_buttons);
		break;
	case HUD:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->hud);
		break;
	case OTHER_TITLES:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->titles_other);
		break;
	case SUBTITLES:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->subtitle);
		break;
	case MAIN_TITLE_BUTTON:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->main_title_buttons);
		break;
	case DEFAULT:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->default);
		break;
	case DEFAULT_BIGGER:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->default_bigger);
		break;
	}
}