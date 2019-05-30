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

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	controller = nullptr;
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		LOG("Init the controller (search and asign)");
		controller = nullptr;
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i)) {
				joystick = SDL_JoystickOpen(0);
				controller = SDL_GameControllerOpen(i);
				if (controller) {
					break;
				}
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

	keyPressed = false;
	scancode = SDL_SCANCODE_UNKNOWN;
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			keyPressed = true;
			scancode = event.key.keysym.scancode; 
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

	buttonForGamepad();

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

	

	return true;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	
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
void ModuleInput::buttonForGamepad() {
	//BUTTON A
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) == 1) {
		if (gamepad.A == PAD_BUTTON_IDLE)
			gamepad.A = PAD_BUTTON_DOWN;
		else
			gamepad.A = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.A == PAD_BUTTON_REPEAT || (gamepad.A == PAD_BUTTON_DOWN))
			gamepad.A = PAD_BUTTON_KEY_UP;
		else
			gamepad.A = PAD_BUTTON_IDLE;
	}

	//BUTTON B
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B) == 1) {
		if (gamepad.B == PAD_BUTTON_IDLE)
			gamepad.B = PAD_BUTTON_DOWN;
		else
			gamepad.B = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.B == PAD_BUTTON_REPEAT || (gamepad.B == PAD_BUTTON_DOWN))
			gamepad.B = PAD_BUTTON_KEY_UP;
		else
			gamepad.B = PAD_BUTTON_IDLE;
	}

	//BUTTON X
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) == 1) {
		if (gamepad.X == PAD_BUTTON_IDLE)
			gamepad.X = PAD_BUTTON_DOWN;
		else
			gamepad.X = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.X == PAD_BUTTON_REPEAT || (gamepad.X == PAD_BUTTON_DOWN))
			gamepad.X = PAD_BUTTON_KEY_UP;
		else
			gamepad.X = PAD_BUTTON_IDLE;
	}

	//BUTTON Y
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y) == 1) {
		if (gamepad.Y == PAD_BUTTON_IDLE)
			gamepad.Y = PAD_BUTTON_DOWN;
		else
			gamepad.Y = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.Y == PAD_BUTTON_REPEAT || (gamepad.Y == PAD_BUTTON_DOWN))
			gamepad.Y = PAD_BUTTON_KEY_UP;
		else
			gamepad.Y = PAD_BUTTON_IDLE;
	}

	//BUTTON START
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START) == 1) {
		if (gamepad.START == PAD_BUTTON_IDLE)
			gamepad.START = PAD_BUTTON_DOWN;
		else
			gamepad.START = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.START == PAD_BUTTON_REPEAT || (gamepad.START == PAD_BUTTON_DOWN))
			gamepad.START = PAD_BUTTON_KEY_UP;
		else
			gamepad.START = PAD_BUTTON_IDLE;
	}


	//BUTTON SELECT
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_GUIDE) == 1) {
		if (gamepad.SELECT == PAD_BUTTON_IDLE)
			gamepad.SELECT = PAD_BUTTON_DOWN;
		
			gamepad.SELECT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.SELECT == PAD_BUTTON_REPEAT || (gamepad.SELECT == PAD_BUTTON_DOWN))
			gamepad.SELECT = PAD_BUTTON_KEY_UP;
		else
			gamepad.SELECT = PAD_BUTTON_IDLE;
	}

	//BUTTON L1
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1) {
		if (gamepad.L1 == PAD_BUTTON_IDLE)
			gamepad.L1 = PAD_BUTTON_DOWN;
		else
			gamepad.L1 = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.L1 == PAD_BUTTON_REPEAT || (gamepad.L1 == PAD_BUTTON_DOWN))
			gamepad.L1 = PAD_BUTTON_KEY_UP;
		else
			gamepad.L1 = PAD_BUTTON_IDLE;
	}


	//BUTTON L2
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1) {
		if (gamepad.L2 == PAD_BUTTON_IDLE)
			gamepad.L2 = PAD_BUTTON_DOWN;
		else
			gamepad.L2 = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.L2 == PAD_BUTTON_REPEAT || (gamepad.L2 == PAD_BUTTON_DOWN))
			gamepad.L2 = PAD_BUTTON_KEY_UP;
		else
			gamepad.L2 = PAD_BUTTON_IDLE;
	}


	//BUTTON R1
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1) {
		if (gamepad.R1 == PAD_BUTTON_IDLE)
			gamepad.R1 = PAD_BUTTON_DOWN;
		else
			gamepad.R1 = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.R1 == PAD_BUTTON_REPEAT || (gamepad.R1 == PAD_BUTTON_DOWN))
			gamepad.R1 = PAD_BUTTON_KEY_UP;
		else
			gamepad.R1 = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD UP
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
		if (gamepad.CROSS_UP == PAD_BUTTON_IDLE)
			gamepad.CROSS_UP = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_UP = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_UP == PAD_BUTTON_REPEAT || (gamepad.CROSS_UP == PAD_BUTTON_DOWN))
			gamepad.CROSS_UP = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_UP = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD DOWN
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
		if (gamepad.CROSS_DOWN == PAD_BUTTON_IDLE)
			gamepad.CROSS_DOWN = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_DOWN = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_DOWN == PAD_BUTTON_REPEAT || (gamepad.CROSS_DOWN == PAD_BUTTON_DOWN))
			gamepad.CROSS_DOWN = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_DOWN = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD LEFT
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
		if (gamepad.CROSS_LEFT == PAD_BUTTON_IDLE)
			gamepad.CROSS_LEFT = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_LEFT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_LEFT == PAD_BUTTON_REPEAT || (gamepad.CROSS_LEFT == PAD_BUTTON_DOWN))
			gamepad.CROSS_LEFT = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_LEFT = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD UP
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
		if (gamepad.CROSS_RIGHT == PAD_BUTTON_IDLE)
			gamepad.CROSS_RIGHT = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_RIGHT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_RIGHT == PAD_BUTTON_REPEAT || (gamepad.CROSS_RIGHT == PAD_BUTTON_DOWN))
			gamepad.CROSS_RIGHT = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_RIGHT = PAD_BUTTON_IDLE;
	}

	//JOYSTICK UP
	if (SDL_JoystickGetAxis(joystick, 1) < -12000) {
		if (gamepad.JOYSTICK_UP == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_UP = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_UP = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_UP == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_UP == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_UP = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_UP = PAD_BUTTON_IDLE;
	}

	//JOYSTICK DOWN
	if (SDL_JoystickGetAxis(joystick, 1) > 12000) {
		if (gamepad.JOYSTICK_DOWN == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_DOWN == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_DOWN == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_IDLE;
	}

	//JOYSTICK LEFT
	if (SDL_JoystickGetAxis(joystick, 0) < -12000) {
		if (gamepad.JOYSTICK_LEFT == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_LEFT == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_LEFT == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_IDLE;
	}

	//JOYSTICK RIGHT
	if (SDL_JoystickGetAxis(joystick, 0) > 12000) {
		if (gamepad.JOYSTICK_RIGHT == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_RIGHT == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_RIGHT == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_IDLE;
	}

}

void ModuleInput::CleanCount() {

	count_repeat = 0;
	count_repeat_mix = 0;
}

bool ModuleInput::Left() {
	
	if ((GetKey(keyboard_buttons.buttons_to_use.LEFT) == KEY_REPEAT 
		|| gamepad.JOYSTICK_LEFT == GAMEPAD_STATE::PAD_BUTTON_REPEAT) && count_repeat > MAX_COUNT_INPUT) 
	{
		CleanCount();
		return true;
	}
	++count_repeat;
	return false;
}

bool ModuleInput::Right() {
	
	if ((GetKey(keyboard_buttons.buttons_to_use.RIGHT) == KEY_REPEAT
		|| gamepad.JOYSTICK_RIGHT == GAMEPAD_STATE::PAD_BUTTON_REPEAT) && count_repeat > MAX_COUNT_INPUT) 
	{
		CleanCount();
		return true;
	}
	++count_repeat;
	return false;
}

bool ModuleInput::Down() {
	
	if ((GetKey(keyboard_buttons.buttons_to_use.DOWN) == KEY_REPEAT
		|| gamepad.JOYSTICK_DOWN == GAMEPAD_STATE::PAD_BUTTON_REPEAT) && count_repeat > MAX_COUNT_INPUT)
	{
		CleanCount();
		return true;
	}
	++count_repeat;
	return false;
}

bool ModuleInput::Up() {
	
	if ((GetKey(keyboard_buttons.buttons_to_use.UP) == KEY_REPEAT
		|| gamepad.JOYSTICK_UP == GAMEPAD_STATE::PAD_BUTTON_REPEAT) && count_repeat > MAX_COUNT_INPUT)
	{
		CleanCount();
		return true;
	}
	++count_repeat;
	return false;
}

bool ModuleInput::LeftUp() {

	if (((GetKey(keyboard_buttons.buttons_to_use.LEFT) == KEY_REPEAT
		&& GetKey(keyboard_buttons.buttons_to_use.UP) == KEY_REPEAT)
		||	(gamepad.JOYSTICK_LEFT == GAMEPAD_STATE::PAD_BUTTON_REPEAT
		&& gamepad.JOYSTICK_UP == GAMEPAD_STATE::PAD_BUTTON_REPEAT))
		&& count_repeat_mix > MAX_COUNT_INPUT_MIX)
	{
		CleanCount();
		return true;
	}
	++count_repeat_mix;
	return false;
}

bool ModuleInput::RightUp() {

	if (((GetKey(keyboard_buttons.buttons_to_use.RIGHT) == KEY_REPEAT
		&& GetKey(keyboard_buttons.buttons_to_use.UP) == KEY_REPEAT)
		|| (gamepad.JOYSTICK_RIGHT == GAMEPAD_STATE::PAD_BUTTON_REPEAT
		&& gamepad.JOYSTICK_UP == GAMEPAD_STATE::PAD_BUTTON_REPEAT))
		&& count_repeat_mix > MAX_COUNT_INPUT_MIX) 
	{
		CleanCount();
		return true;
	}
	++count_repeat_mix;
	return false;
}

bool ModuleInput::LeftDown() {

	if (((GetKey(keyboard_buttons.buttons_to_use.LEFT) == KEY_REPEAT
		&& GetKey(keyboard_buttons.buttons_to_use.DOWN) == KEY_REPEAT)
		|| (gamepad.JOYSTICK_LEFT == GAMEPAD_STATE::PAD_BUTTON_REPEAT
		&& gamepad.JOYSTICK_DOWN == GAMEPAD_STATE::PAD_BUTTON_REPEAT))
		&& count_repeat_mix > MAX_COUNT_INPUT_MIX)
	{
		CleanCount();
		return true;
	}
	++count_repeat_mix;
	return false;
}

bool ModuleInput::RightDown() {

	if (((GetKey(keyboard_buttons.buttons_to_use.RIGHT) == KEY_REPEAT
		&& GetKey(keyboard_buttons.buttons_to_use.DOWN) == KEY_REPEAT)
		|| (gamepad.JOYSTICK_RIGHT == GAMEPAD_STATE::PAD_BUTTON_REPEAT
		&& gamepad.JOYSTICK_DOWN == GAMEPAD_STATE::PAD_BUTTON_REPEAT))
		&& count_repeat_mix > MAX_COUNT_INPUT_MIX)
	{
		CleanCount();
		return true;
	}
	++count_repeat_mix;
	return false;
}

bool ModuleInput::Accept() {

	if (GetKey(keyboard_buttons.buttons_to_use.ACCEPT) == KEY_DOWN ||gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		return true;
	}


	return false;
}

bool ModuleInput::Decline() {

	if (GetKey(keyboard_buttons.buttons_to_use.DECLINE) == KEY_DOWN || gamepad.B == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		return true;
	}

	return false;
}

bool ModuleInput::Pause() {

	if (GetKey(keyboard_buttons.buttons_to_use.PAUSE) == KEY_DOWN || App->input->gamepad.START == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		return true;
	}
	return false;
}

bool ModuleInput::LevelUp() {

	if (GetKey(keyboard_buttons.buttons_to_use.LEVELUP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		return true;
	}
	return false;
}

bool ModuleInput::LevelDown() {

	if (GetKey(keyboard_buttons.buttons_to_use.LEVELDOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		return true;
	}
	return false;
}

bool ModuleInput::AllMaps() {

	if (GetKey(keyboard_buttons.buttons_to_use.ALLMAPS) == KEY_DOWN || App->input->gamepad.CROSS_RIGHT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		return true;
	}
	return false;
}

void ModuleInput::Defaultcontrols() {


	keyboard_buttons.buttons_to_use.ACCEPT = SDL_SCANCODE_SPACE;
	keyboard_buttons.buttons_to_use.RIGHT = SDL_SCANCODE_RIGHT;
	keyboard_buttons.buttons_to_use.LEFT = SDL_SCANCODE_LEFT;
	keyboard_buttons.buttons_to_use.DOWN = SDL_SCANCODE_DOWN;
	keyboard_buttons.buttons_to_use.UP = SDL_SCANCODE_UP;
	keyboard_buttons.buttons_to_use.DECLINE = SDL_SCANCODE_X;
	keyboard_buttons.buttons_to_use.PAUSE = SDL_SCANCODE_ESCAPE;
	keyboard_buttons.buttons_to_use.LEVELUP = SDL_SCANCODE_F4;
	keyboard_buttons.buttons_to_use.LEVELDOWN = SDL_SCANCODE_F3;
	keyboard_buttons.buttons_to_use.ALLMAPS = SDL_SCANCODE_F2;


	keyboard_buttons.buttons_char.RIGHT == "RIGHT ARROW";
	keyboard_buttons.buttons_char.LEFT == "LEFT ARROW";
	keyboard_buttons.buttons_char.DOWN == "DOWN ARROW";
	keyboard_buttons.buttons_char.UP == "UP ARROW";
	keyboard_buttons.buttons_char.ACCEPT == "SPACE";
	keyboard_buttons.buttons_char.DECLINE == "X";
	keyboard_buttons.buttons_char.PAUSE == "ESC";
}


void ModuleInput::GetKeyPressed() {

	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			scancode = event.key.keysym.scancode;
		}

	}

}

//bool ModuleInput::Select() {
//
//	if (GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN || App->input->gamepad.SELECT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
//		return true;
//	}
//	return false;
//}
//
//bool ModuleInput::Showcharacterstats() {
//
//	if (GetKey(SDL_SCANCODE_LCTRL) == KEY_DOWN || App->input->gamepad.L1 == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
//		return true;
//	}
//	return false;
//}
//
//bool ModuleInput::Showcharacterabilities() {
//
//	if (GetKey(SDL_SCANCODE_LALT) == KEY_DOWN || App->input->gamepad.L2 == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
//		return true;
//	}
//	return false;
//}
//
//bool ModuleInput::Showabilities() {
//
//	if (GetKey(SDL_SCANCODE_TAB) == KEY_DOWN || App->input->gamepad.R1 == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
//		return true;
//	}
//	return false;
//}

