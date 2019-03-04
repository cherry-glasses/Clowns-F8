#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include "Defs.h"
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml\src\pugixml.hpp"

//Modules
typedef std::list<Module*> ModuleList;
class ModuleWindow;
class ModuleRender;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleFonts;
class ModuleMap;
class ModuleEntityManager;
class ModuleGUIManager;
class Scene;


class Application
{

public:

	// Constructor
	Application(int argc, char* args[]);

	// Destructor
	virtual ~Application();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(std::list<std::string>& list_to_fill) const;

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

private:

	ModuleList			modules;
	float				dt = .0f;
	int					argc;
	char**				args;
	std::string			title;
	std::string			organization;
	uint32				frame_rate;
	std::string			load_game;
	std::string			save_game;

	bool				want_to_save = false;
	bool				want_to_load = false;
	bool				want_to_quit = false;

	int					aux_seconds = 0;
	Timer				startup_time;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	uint64				frame_count = 0;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				seconds_since_startup;

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__