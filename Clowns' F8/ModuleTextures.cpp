#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include <string.h>

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleTextures::ModuleTextures() : Module()
{
	name = "textures";
}

// Destructor
ModuleTextures::~ModuleTextures()
{}

// Called before render is available
bool ModuleTextures::Awake(pugi::xml_node& _config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool ModuleTextures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");
	std::list<SDL_Texture*>::iterator item;

	for (item = textures.begin(); item != textures.end(); item++)
	{
		SDL_DestroyTexture(*item);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* _path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(_path);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", _path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool ModuleTextures::UnLoad(SDL_Texture* _texture)
{
	std::list<SDL_Texture*>::iterator item;

	for (item = textures.begin(); item != textures.end(); item++)
	{
		if (_texture == *item)
		{
			SDL_DestroyTexture(*item);
			textures.remove(*item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const ModuleTextures::LoadSurface(SDL_Surface* _surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, _surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_front(texture);
	}

	return texture;
}

// Retrieve size of a texture
void ModuleTextures::GetSize(const SDL_Texture* _texture, uint& _width, uint& _height) const
{
	SDL_QueryTexture((SDL_Texture*)_texture, NULL, NULL, (int*)&_width, (int*)&_height);
}