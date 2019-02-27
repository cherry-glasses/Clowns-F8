#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL_scancode.h"
#include "SDL/include/SDL_gamecontroller.h"

#define MAX_KEYS 300


enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

public:
	SDL_GameController * controller;
	KEY_STATE keyboard[MAX_KEYS];
	KEY_STATE controller_A_button;
	KEY_STATE controller_Y_button;
	KEY_STATE controller_START_button;
};

#endif // __ModuleInput_H__