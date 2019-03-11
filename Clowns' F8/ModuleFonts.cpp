#include "Log.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"

#include "SDL\include\SDL.h"
#include "SDL_ttf\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )


// Constructor
ModuleFonts::ModuleFonts() : Module()
{
	name = "fonts";
}

// Destructor
ModuleFonts::~ModuleFonts()
{}

// Called before render is available
bool ModuleFonts::Awake(pugi::xml_node& _conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = _conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = _conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default_font = Load(path, size);
	}

	return ret;
}

// Called before quitting
bool ModuleFonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	std::list<TTF_Font*>::iterator item;

	for (item = fonts.begin(); item != fonts.end(); item++)
	{
		TTF_CloseFont(*item);
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const ModuleFonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

bool ModuleFonts::Unload(_TTF_Font * font)
{
	bool ret = true;
	if (font != nullptr) {
		TTF_CloseFont(font);
		fonts.remove(font);
	}
	else ret = false;
	return ret;
}

// Print text using font
SDL_Texture* ModuleFonts::Print(const char* _text, SDL_Color _color, TTF_Font* _font)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((_font) ? _font : default_font, _text, _color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->textures->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool ModuleFonts::CalcSize(const char* _text, int& _width, int& _height, _TTF_Font* _font) const
{
	bool ret = false;

	if (TTF_SizeText((_font) ? _font : default_font, _text, &_width, &_height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}