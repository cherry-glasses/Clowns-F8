#include "Log.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

ModuleRender::ModuleRender() : Module()
{
	name = "render";
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Awake(pugi::xml_node& _config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (_config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->window->screen_surface->w;
		camera.h = App->window->screen_surface->h;
		camera.x = _config.child("camera").attribute("x").as_int();
		camera.y = _config.child("camera").attribute("y").as_int();
	}

	return ret;
}

// Called before the first frame
bool ModuleRender::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	return true;
}

// Called each loop iteration
bool ModuleRender::PreUpdate()
{
	SDL_RenderClear(renderer);

	return true;
}

bool ModuleRender::Update(float _dt)
{
	return true;
}

bool ModuleRender::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool ModuleRender::Load(pugi::xml_node& _data)
{
	return true;
}

// Save Game State
bool ModuleRender::Save(pugi::xml_node& _data) const
{
	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* _texture, int _x, int _y, const SDL_Rect* _section, float _speed, bool _flipX, double _angle, int _pivot_x, int _pivot_y) const
{
	bool ret = true;
	uint scale = App->window->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * _speed) + _x * scale;
	rect.y = (int)(camera.y * _speed) + _y * scale;

	if (_section != NULL)
	{
		rect.w = _section->w;
		rect.h = _section->h;
	}
	else
	{
		SDL_QueryTexture(_texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	//Don't blit if not on screen
	uint width, height = 0;
	App->window->GetWindowSize(width, height);

	if (rect.x + 100 + rect.w < 0 || rect.y + rect.h < 0 || rect.x >(int)width || rect.y >(int)height)
	{
		return false;
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (_pivot_x != INT_MAX && _pivot_y != INT_MAX)
	{
		pivot.x = _pivot_x;
		pivot.y = _pivot_y;
		p = &pivot;
	}

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (_flipX) 
	{
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (SDL_RenderCopyEx(renderer, _texture, _section, &rect, _angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}


