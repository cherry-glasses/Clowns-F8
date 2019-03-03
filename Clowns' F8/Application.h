#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml\src\pugixml.hpp"

#define NUM_MODULES 11

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleFonts;
class ModuleMap;
class ModuleEntityManager;
class ModuleGUIManager;
class Module;
class Scene;



class Application
{

public:

	Application();
	~Application();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();


	bool Init();
	update_status Update();
	bool CleanUp();

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

	void GetContinueState();


private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;


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
	ModuleEntityManager* entity_manager = nullptr;
	ModuleGUIManager* gui_manager = nullptr;


};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__