#ifndef __GUILabel_H__
#define __GUILabel_H__

#include "GUIElement.h"
#include "ModuleFonts.h"
#include "p2SString.h"
#include "ModuleTextures.h"

#include "SDL/include/SDL_pixels.h"

struct _TTF_Font;
struct SDL_Texture;

class GUILabel : public GUIElement
{
public:
	GUILabel(int x, int y, p2SString text, SDL_Color color, _TTF_Font* font, GUIElement* son);
	~GUILabel()
	{
		App->textures->Unload(tex);
	};

	void Update(float dt);
	void DrawLabel();

	void SetText(p2SString text);
	void AddChar(p2SString c);
	void DeleteLastChar();

public:
	_TTF_Font* font = nullptr;
	SDL_Texture* tex = nullptr;
	p2SString text;
	SDL_Color color;
};

#endif // __GUILabel_H__