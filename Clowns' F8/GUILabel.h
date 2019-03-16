//#ifndef __guilabel_h__
//#define __guilabel_h__
//
//#include "GUIElement.h"
//#include "ModuleFonts.h"
//#include "ModuleTextures.h"
//
//#include "SDL/include/SDL_pixels.h"
//
//struct _ttf_font;
//struct sdl_texture;
//
//class GUILabel : public GUIElement
//{
//public:
//	GUILabel(int x, int y, std::string text, SDL_Color color, _ttf_font* font, GUIElement* son);
//	~GUILabel()
//	{
//		App->textures->UnLoad(tex);
//	};
//
//	void Update(float dt);
//	void DrawLabel();
//
//	void SetText(std::string text);
//	void AddChar(std::string c);
//	void DeleteLastChar();
//
//public:
//	_ttf_font* font = nullptr;
//	SDL_Texture* tex = nullptr;
//	std::string text;
//	SDL_Color color;
//};
//
//#endif // __guilabel_h__