#ifndef __GUILabel_H__
#define __GUILabel_H__

#include "GUIElement.h"
#include "modulefonts.h"
#include "moduletextures.h"
#include "Application.h"
#include "ModuleTextures.h"

#include "SDL\include\SDL_pixels.h"

class GUILabel : public GUIElement
{
public:
	GUILabel(int x, int y, std::string text, SDL_Color color, _TTF_Font* font, int curr, int def, GUIElement* son);
	~GUILabel()
	{
		App->textures->UnLoad(tex);
	};

	void Update(float dt);
	void DrawLabel();

	void SetText(std::string text);
	void AddChar(std::string c);
	void DeleteLastChar();
public:
	_TTF_Font* font = nullptr;
	SDL_Texture* tex = nullptr;
	std::string text;
	SDL_Color color;
};

#endif // __GUILabel_H__