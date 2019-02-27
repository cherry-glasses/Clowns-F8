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
	~ModuleRender();
	
	bool Init();
	update_status PostUpdate();
	update_status Update();
	update_status PreUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed = 1.0f, bool use_camera = true);
	bool BlitFlipped(SDL_Texture* texture, int x, int y, SDL_Rect* section, bool flipX, bool flipY, double angle = 0.0, SDL_Point center = { 0,0 }, float speed = 1.0f, bool use_camera = true);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);
	void setAlpha(SDL_Texture* texture, int alpha);

	void ResetCamera();
	void SetCameraPosition(int x, int y);

public:
	SDL_Renderer * renderer = nullptr;
	SDL_Rect camera;
};

#endif //__ModuleRender_H__