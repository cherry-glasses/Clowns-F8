#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
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
	class ModuleInput : public Module
	{
	public:

		ModuleInput();

		// Destructor
		virtual ~ModuleInput();

		// Called before render is available
		bool Awake(pugi::xml_node&) override;

		// Called before the first frame
		bool Start() override;

		// Called each loop iteration
		bool PreUpdate() override;

		bool Update(float dt) override;

		// Called before quitting
		bool CleanUp(pugi::xml_node&) override;


public:
	SDL_GameController * controller;
	KEY_STATE keyboard[MAX_KEYS];
	KEY_STATE controller_A_button;
	KEY_STATE controller_Y_button;
	KEY_STATE controller_START_button;
};

#endif // __ModuleInput_H__