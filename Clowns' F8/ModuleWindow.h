#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

public:
	//The window we'll be rendering to
	SDL_Window * window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	uint		width = 0u;
	uint		height = 0u;
	uint		scale = 0u;
};

#endif // __ModuleWindow_H__