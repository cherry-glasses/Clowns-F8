#include "Application.h"
#include "ModuleGUIManager.h"
#include "GUIElement.h"
#include "GUILabel.h"
#include "ModuleRender.h"

#include "SDL_ttf/include/SDL_ttf.h"

GUILabel::GUILabel(int x, int y, std::string text, SDL_Color color, _TTF_Font* font, int curr, int def, int line_width, GUIElement* son, bool centrated) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_LABEL;
	int width = 0, height = 0;
	App->fonts->CalcSize("a", width, height, App->gui_manager->default_font_used);

	this->color = color;
	this->font = font;

	if (curr == -1000 && def == -1000)
	{
		this->text = text;
		int lines = 1;
		int chars = 0;
		int max_chars = line_width / width;
		for (uint i = 0; i < this->text.size(); ++i)
		{
			if (this->text.data()[i] == '\n' || chars > max_chars)
			{
				chars = 0;
				++lines;

			}
			++chars;
		}
		area.w = line_width;
		area.h = height * lines;
	}
	else
	{
		this->text = std::to_string(curr);
		this->text.push_back('/');
		this->text += std::to_string(def);
		area.w = width;
		area.h = height;
	}
	tex = App->fonts->Print(this->text.data(), this->color, this->font, area.w);

	area.x = 0;
	area.y = 0;

	if (centrated)
	{
		position.first = x - (area.w / 2);
		position.second = y - (area.h / 2);
	}
	else
	{
		position.first = x;
		position.second = y;
	}

}

void GUILabel::Update(float dt)
{
	Move();
}

void GUILabel::DrawLabel()
{

	App->render->Blit(tex, position.first, position.second, &area, 0.0f);
	SDL_DestroyTexture(tex);
}

// Note: see SDL_RenderSetClip() for clipping
void GUILabel::SetText(std::string text)
{
	/*this->text = text;

	texts.begin() = App->fonts->Print(this->text.data(), this->color, this->font);

	// Create new rect
	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.data(), width, height, App->gui_manager->default_font_used);
	area.w = width;
	area.h = height;

	area.x = 0;
	area.y = 0;*/
}

void GUILabel::AddChar(std::string c)
{
	/*this->text += c;

	tex = App->fonts->Print(this->text.data(), this->color, this->font);

	// Create new rect
	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.data(), width, height, App->gui_manager->default_font_used);
	area.w = width;
	area.h = height;

	area.x = 0;
	area.y = 0;*/
}

void GUILabel::DeleteLastChar()
{
	if (this->text.length() > 0)
	{
		char* temp_text = (char*)(this->text.data());
		temp_text[this->text.length() - 1] = '\0';

		this->text = temp_text;
	}

	// Create new rect
	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.data(), width, height, App->gui_manager->default_font_used);
	area.w = width;
	area.h = height;

	area.x = 0;
	area.y = 0;
}