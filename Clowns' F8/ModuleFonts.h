#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define DEFAULT_FONT "Assets/Fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 30

struct SDL_Texture;
struct _TTF_Font;

class ModuleFonts : public Module
{
public:

	ModuleFonts();

	// Destructor
	virtual ~ModuleFonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* _path, int _size = 12);
	bool Unload(_TTF_Font* _font);

	// Create a surface from text
	SDL_Texture* Print(const char* _text, SDL_Color _color = { 255, 255, 255, 255 }, _TTF_Font* _font = NULL);

	bool CalcSize(const char* _text, int& _width, int& _height, _TTF_Font* _font = NULL) const;

public:

	std::list<_TTF_Font*>	fonts;
	_TTF_Font*				default_font;
};


#endif // __ModuleFonts_H__
