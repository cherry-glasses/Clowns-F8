#include "Log.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"

ModuleInput::ModuleInput() : Module()
{
	name = "input";

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * NUM_MOUSE_BUTTONS);
	
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Awake(pugi::xml_node& _config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	controller = nullptr;
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i)) 
		{
			controller = SDL_GameControllerOpen(i);
			if (controller) {
				break;
			}
			else {
				LOG("Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}

	return ret;
}

// Called before the first frame
bool ModuleInput::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool ModuleInput::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[EW_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[EW_HIDE] = true;
				break;

			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[EW_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			int scale = App->window->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			break;
		}
	}


	InputGamepad();

	gamepad.left_joystick.first = ((float)SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f);
	gamepad.left_joystick.second = ((float)SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f);

	//JOYSTICK
	if (gamepad.left_joystick.first > 0.25)
	{
		joystick_right = true;
		if (time == 8) {
			joystick_right_repeat = true;
			time = 0;
		}
		else
			joystick_right_repeat = false;

		time++;
	}
	else {
		joystick_right = false;
		joystick_right_repeat = false;
	}

	if (gamepad.left_joystick.first < -0.25)
	{
		joystick_left = true;
		if (time == 8) {
			joystick_left_repeat = true;
			time = 0;
		}
		else
			joystick_left_repeat = false;

		time++;
	}
	else {
		joystick_left = false;
		joystick_left_repeat = false;
	}

	joystick_down = 0;
	joystick_up = 0;

	if (gamepad.left_joystick.second > 0.25)
	{
		joystick_down = true;
	}
	else
		joystick_down = false;
	if (gamepad.left_joystick.second < -0.25)
	{
		joystick_up = true;
	}
	else
		joystick_up = false;



	return true;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i)) {
			SDL_GameController *controller = SDL_GameControllerOpen(i);
			if (controller)
			{
				SDL_GameControllerClose(controller);
			}
			else 
			{
				LOG("Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EVENT_WINDOW _ev)
{
	return windowEvents[_ev];
}

void ModuleInput::GetMousePosition(int& _x, int& _y)
{
	_x = mouse_x;
	_y = mouse_y;
}

void ModuleInput::GetMouseMotion(int& _x, int& _y)
{
	_x = mouse_motion_x;
	_y = mouse_motion_y;
}
void ModuleInput::InputGamepad() {
	//BUTTON A
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) == 1) {
		if (gamepad.A == BUTTON_IDLE)
			gamepad.A = BUTTON_DOWN;
		else
			gamepad.A = BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.A == BUTTON_REPEAT || (gamepad.A == BUTTON_DOWN))
			gamepad.A = BUTTON_UP;
		else
			gamepad.A = BUTTON_IDLE;
	}

	//BUTTON START
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START) == 1) {
		if (gamepad.START == BUTTON_IDLE)
			gamepad.START = BUTTON_DOWN;
		else
			gamepad.START = BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.START == BUTTON_REPEAT || (gamepad.START == BUTTON_DOWN))
			gamepad.START = BUTTON_UP;
		else
			gamepad.START = BUTTON_IDLE;
	}

	//BUTTON DPAD UP
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
		if (gamepad.DPAD_UP == BUTTON_IDLE)
			gamepad.DPAD_UP = BUTTON_DOWN;
		else
			gamepad.DPAD_UP = BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.DPAD_UP == BUTTON_REPEAT || (gamepad.DPAD_UP == BUTTON_DOWN))
			gamepad.DPAD_UP = BUTTON_UP;
		else
			gamepad.DPAD_UP = BUTTON_IDLE;
	}

	//BUTTON DPAD DOWN
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
		if (gamepad.DPAD_DOWN == BUTTON_IDLE)
			gamepad.DPAD_DOWN = BUTTON_DOWN;
		else
			gamepad.DPAD_DOWN = BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.DPAD_DOWN == BUTTON_REPEAT || (gamepad.DPAD_DOWN == BUTTON_DOWN))
			gamepad.DPAD_DOWN = BUTTON_UP;
		else
			gamepad.DPAD_DOWN = BUTTON_IDLE;
	}

	//BUTTON DPAD LEFT
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
		if (gamepad.DPAD_LEFT == BUTTON_IDLE)
			gamepad.DPAD_LEFT = BUTTON_DOWN;
		else
			gamepad.DPAD_LEFT = BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.DPAD_LEFT == BUTTON_REPEAT || (gamepad.DPAD_LEFT == BUTTON_DOWN))
			gamepad.DPAD_LEFT = BUTTON_UP;
		else
			gamepad.DPAD_LEFT = BUTTON_IDLE;
	}

	//BUTTON DPAD UP
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
		if (gamepad.DPAD_RIGHT == BUTTON_IDLE)
			gamepad.DPAD_RIGHT = BUTTON_DOWN;
		else
			gamepad.DPAD_RIGHT = BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.DPAD_RIGHT == BUTTON_REPEAT || (gamepad.DPAD_RIGHT == BUTTON_DOWN))
			gamepad.DPAD_RIGHT = BUTTON_UP;
		else
			gamepad.DPAD_RIGHT = BUTTON_IDLE;
	}
}
