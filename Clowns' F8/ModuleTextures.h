#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include "Module.h"

struct SDL_Texture;
struct SDL_Surface;

class ModuleTextures : public Module
{
public:

	ModuleTextures();

	// Destructor
	virtual ~ModuleTextures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* _path);
	bool				UnLoad(SDL_Texture* _texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* _surface);
	void				GetSize(const SDL_Texture* _texture, uint& _width, uint& _height) const;

public:

	std::list<SDL_Texture*>	textures;
};

#endif // __ModuleTextures_H__