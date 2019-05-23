#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "SDL\include\SDL_scancode.h"
#include "SDL/include/SDL.h"

#define NUM_MOUSE_BUTTONS 5
#define MAX_KEYS 300

enum EVENT_WINDOW
{
	EW_QUIT = 0,
	EW_HIDE = 1,
	EW_SHOW = 2,
	EW_COUNT
};

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum CONTROLLER_STATE
{
	BUTTON_IDLE = 0,
	BUTTON_DOWN,
	BUTTON_REPEAT,
	BUTTON_UP
};

enum GAMEPAD_STATE
{
	PAD_BUTTON_IDLE = 0,
	PAD_BUTTON_DOWN,
	PAD_BUTTON_REPEAT,
	PAD_BUTTON_KEY_UP
};

struct Gamepad {

	GAMEPAD_STATE A = PAD_BUTTON_IDLE;
	GAMEPAD_STATE B;
	GAMEPAD_STATE Y;
	GAMEPAD_STATE X;

	GAMEPAD_STATE CROSS_UP;
	GAMEPAD_STATE CROSS_DOWN;
	GAMEPAD_STATE CROSS_LEFT;
	GAMEPAD_STATE CROSS_RIGHT;

	GAMEPAD_STATE START;
	GAMEPAD_STATE SELECT;
	GAMEPAD_STATE L1;
	GAMEPAD_STATE L2;
	GAMEPAD_STATE R1;

	GAMEPAD_STATE JOYSTICK_UP;
	GAMEPAD_STATE JOYSTICK_DOWN;
	GAMEPAD_STATE JOYSTICK_LEFT;
	GAMEPAD_STATE JOYSTICK_RIGHT;

	

};

struct ButtonsToUse {

	int UP;
	int DOWN;
	int LEFT;
	int RIGHT;
	int DECLINE;
	int ACCEPT;
	int PAUSE;
	int STATS;
	int CHARACABILITY;
	int ABILITY;
	int ABILITYTREE;
	
	

};


struct ButtonChar {
	char* UP = nullptr;
	char* DOWN = nullptr;
	char* LEFT = nullptr;
	char* RIGHT = nullptr;
	char* DECLINE = nullptr;
	char* ACCEPT = nullptr;
	char* PAUSE = nullptr;
	char* STATS = nullptr;
	char* ABILITY = nullptr;
	char* CHARACABILITY = nullptr;
	char* ABILITYTREE = nullptr;
};

struct KeyboardButtons {
	ButtonsToUse buttons_to_use;
	ButtonChar buttons_char;
};

struct ControllerButtons {
	ButtonsToUse buttons_to_use;
	ButtonChar buttons_char;
};

class ModuleInput : public Module
{
public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(EVENT_WINDOW _ev);

	// Check key states (includes mouse and joy buttons)
	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButtonDown(int _id) const
	{
		return mouse_buttons[_id - 1];
	}


	GAMEPAD_STATE GetGamepadButton(int key)
	{
		if (key == 0)
			return gamepad.A;
		else if (key == 1)
			return gamepad.B;
		else if (key == 2)
			return gamepad.Y;
		else if (key == 3)
			return gamepad.X;

	}



	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	Gamepad gamepad;
	void buttonForGamepad();
	void Defaultcontrols();

	void GetKeyPressed();

	bool Left();
	bool Right();
	bool Down();
	bool Up();
	bool Accept();
	bool Decline();
	bool Pause();
	//bool Select();
	//bool Showcharacterstats();
	//bool Showcharacterabilities();
	//bool Showabilities();
	//

	int space;

	KeyboardButtons keyboard_buttons;
	KEY_STATE*	keyboard;
	SDL_Scancode scancode;
	bool keyPressed = false;

private:
	bool		windowEvents[EW_COUNT];

	KEY_STATE	mouse_buttons[NUM_MOUSE_BUTTONS];

	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	SDL_GameController* controller;
	SDL_Joystick *joystick;
};

#endif // __ModuleInput_H__