#include "Log.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Scene.h"
#include "ModuleSceneManager.h"
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

bool ModuleRender::PostUpdate(float _dt)
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
	if (App->scene_manager->current_scene->type == SCENE_TYPE::FIRST_BATTLE
		|| App->scene_manager->current_scene->type == SCENE_TYPE::SECOND_BATTLE
		|| App->scene_manager->current_scene->type == SCENE_TYPE::THIRD_BATTLE) {
		rect.x = (int)(camera.x * _speed) + (_x + App->window->GetScreenWidth() / 2) * scale;
		rect.y = (int)(camera.y * _speed) + (_y + App->window->GetScreenHeight() / 8) * scale;
	}
	else {
		rect.x = (int)(camera.x * _speed) + _x * scale;
		rect.y = (int)(camera.y * _speed) + _y * scale;
	}
	

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
	uint width = 0;
	uint height = 0;
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

//bool ModuleRender::BlitParticle(SDL_Texture * texture, int x, int y, const SDL_Rect * section, const SDL_Rect * rectSize, SDL_Color color, SDL_BlendMode blendMode, float speed, double angle, int pivot_x, int pivot_y) const
//{
//	bool ret = true;
//	uint scale = App->window->GetScale();
//
//	SDL_Rect rect;
//	rect.x = (int)(camera.first * speed) + x * scale;
//	rect.y = (int)(camera.second * speed) + y * scale;
//
//	if (rectSize != NULL)
//	{
//		rect.w = rectSize->w;
//		rect.h = rectSize->h;
//	}
//	else if (section != NULL)
//	{
//		rect.w = section->w;
//		rect.h = section->h;
//	}
//	else
//		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
//
//	int px = rect.w / 2;
//	int py = rect.h / 2;
//
//	rect.w *= scale;
//	rect.h *= scale;
//
//	SDL_Point* p = NULL;
//	SDL_Point pivot;
//	pivot.x = px;
//	pivot.y = py;
//	p = &pivot;
//
//	if (SDL_SetTextureColorMod(texture, color.r, color.g, color.b) != 0)
//		LOG("Cannot set texture color mode. SDL_SetTextureColorMod error: %s", SDL_GetError());
//
//	if (SDL_SetTextureAlphaMod(texture, color.a) != 0)
//		LOG("Cannot set texture alpha mode. SDL_SetTextureAlphaMod error: %s", SDL_GetError());
//
//	if (SDL_SetTextureBlendMode(texture, blendMode) != 0)
//		LOG("Cannot set texture blend mode. SDL_SetTextureBlendMode error: %s", SDL_GetError());
//
//
//	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, NULL, SDL_FLIP_NONE) != 0)
//	{
//		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
//		ret = false;
//	}
//
//	return ret;
//}



bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

