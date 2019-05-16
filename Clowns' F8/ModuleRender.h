#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Renderer;
struct SDL_Texture;

class ModuleRender : public Module
{
public:

	ModuleRender();

	// Destructor
	virtual ~ModuleRender();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float _dt);
	bool PostUpdate(float _dt);
	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	bool Blit(SDL_Texture* _texture, int _x, int _y, const SDL_Rect* _section = NULL, float _speed = 1.0f, bool _flipX = false, double _angle = 0, int _pivot_x = INT_MAX, int _pivot_y = INT_MAX) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;

public:

	SDL_Renderer*   renderer;
	SDL_Rect		camera = { 0,0,0,0 };
	SDL_Rect		viewport = { 0,0,0,0 };
	SDL_Color		background = { 0,0,0,0 };
};

#endif //__ModuleRender_H__