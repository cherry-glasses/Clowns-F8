#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 10

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleFonts;
class Module;
class Scene;


class Application
{
public:

	Module * modules[NUM_MODULES] = { nullptr };
	ModuleWindow* window = nullptr;
	ModuleRender* render = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleFadeToBlack* fade_to_black = nullptr;
	ModuleFonts* fonts = nullptr;
	Scene* scene = nullptr;
	ModuleMap* map = nullptr;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__