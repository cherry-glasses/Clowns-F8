#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"

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

struct Gamepad 
{
	CONTROLLER_STATE A;
	CONTROLLER_STATE B;
	CONTROLLER_STATE Y;
	CONTROLLER_STATE X;
	CONTROLLER_STATE START;
	CONTROLLER_STATE DPAD_UP;
	CONTROLLER_STATE DPAD_DOWN;
	CONTROLLER_STATE DPAD_LEFT;
	CONTROLLER_STATE DPAD_RIGHT;
	std::pair<float, float> left_joystick;
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

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);
	void InputGamepad();

private:
	bool		windowEvents[EW_COUNT];
	KEY_STATE*	keyboard;
	KEY_STATE	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	int time = 0;
};

#endif // __ModuleInput_H__