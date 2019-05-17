#include "Log.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleMap.h"
#include "ModuleSceneManager.h"
#include "ModuleTransitionManager.h"
#include "ModuleEntityManager.h"
#include "ModuleGUIManager.h"
#include "ModulePathfinding.h"
#include "ModuleParticleSystem.h"



#include "SDL\include\SDL_timer.h"


Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{
	window = new ModuleWindow();
	render = new ModuleRender();
	input = new ModuleInput();
	textures = new ModuleTextures();
	audio = new ModuleAudio();
	fonts = new ModuleFonts();
	map = new ModuleMap();
	scene_manager = new ModuleSceneManager();
	entity_manager = new ModuleEntityManager();
	gui_manager = new ModuleGUIManager();
	pathfinding = new ModulePathfinding();
	transition_manager = new ModuleTransitionManager();
//	particle_system = new ModuleParticleSystem();
	

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(window);
	AddModule(input);
	AddModule(textures);
	AddModule(audio);
	AddModule(fonts);
	AddModule(map);
	AddModule(pathfinding);
	AddModule(scene_manager);
	AddModule(entity_manager);
	AddModule(gui_manager);
	AddModule(transition_manager);
//	AddModule(particle_system);

	// render last to swap buffer
	AddModule(render);

}

Application::~Application()
{
	for (ModuleList::reverse_iterator item = modules.rbegin(); item != modules.rend(); item++) {
		RELEASE(*item);
	}
	modules.clear();
}

void Application::AddModule(Module* _module)
{
	_module->Init();
	modules.push_back(_module);
}

// ---------------------------------------------
pugi::xml_node Application::LoadConfig(pugi::xml_document& _config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = _config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = _config_file.child("config");

	return ret;
}
pugi::xml_node Application::LoadEmitters(pugi::xml_document& psystem_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = psystem_file.load_file("psystem_config.xml");

	if (result == NULL)
		LOG("Could not load xml file config.xml. Pugi error: %s", result.description());
	else
		ret = psystem_file.child("emitters");
	return ret;
}

bool Application::Awake()
{
	bool ret = false;

	
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title = (app_config.child("title").child_value());
		organization = (app_config.child("organization").child_value());
		frame_rate = app_config.child("framerate_cap").attribute("rate").as_uint();
		save_game = (app_config.child("save").child_value());
		load_game = save_game;

	}

	if (ret == true)
	{
		for (ModuleList::iterator item = modules.begin(); item != modules.end() && ret == true; item++) {
			pugi::xml_node node = config.child((*item)->name.c_str());
			ret = (*item)->Awake(node);
		}
	}

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	for (ModuleList::iterator item = modules.begin(); item != modules.end() && ret == true; item++) {
		ret = (*item)->Start();
	}

	return ret;
}

bool Application::Update()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}

	PrepareUpdate();

	if (want_to_quit == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();



	FinishUpdate();

	return ret;
}

void Application::PrepareUpdate()
{
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	if (dt > frame_rate / 1000)
		dt = frame_rate / 1000;

	frame_time.Start();
}


void Application::FinishUpdate()
{
	if (want_to_save == true) 
	{
		SavegameNow();
	}
		
	if (want_to_load == true)
	{
		LoadGameNow();
	}
		
	// Framerate calculations --
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	double last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	//BROFILER_CATEGORY("Waiting", Profiler::Color::SeaGreen);

	static char title[256];
	sprintf_s(title, 256, " FPS: %i Last Frame Ms: %.2f",
		frames_on_last_update, last_frame_ms);
	App->window->SetTitle(title);
	
	float waiting_time = (1000 / frame_rate);
	if (last_frame_ms < waiting_time)
	{
		waiting_time -= last_frame_ms;
		SDL_Delay(waiting_time);
	}
}

bool Application::PreUpdate()
{
	bool ret = true;

	ModuleList::iterator item;
	item = modules.begin();
	Module* p_module = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		p_module = *item;

		if (p_module->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

bool Application::DoUpdate()
{
	bool ret = true;
	Module* p_module = NULL;

	for (ModuleList::iterator item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		p_module = *item;

		if (p_module->active == false) {
			continue;
		}

		ret = p_module->Update(dt);
	}

	return ret;
}

bool Application::PostUpdate()
{
	bool ret = true;
	ModuleList::iterator item;
	Module* p_module = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		p_module = *item;

		if (p_module->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate(dt);
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (ModuleList::reverse_iterator item = modules.rbegin(); item != modules.rend() && ret == true; item++)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

// Load / Save
void Application::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void Application::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?
	want_to_save = true;
}

// ---------------------------------------
void Application::GetSaveGames(std::list<std::string>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool Application::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file((char*)load_game.c_str());

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.c_str());

		root = data.child("game_state");


		ret = true;
		ModuleList::iterator item;

		for (item = modules.begin(); item != modules.end() && ret == true; item++)
		{
			pugi::xml_node node = root.child((*item)->name.c_str());
			ret = (*item)->Load(node);
		}

		data.reset();
		if (ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (*item != nullptr) ? (*item)->name.c_str() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.c_str(), result.description());

	want_to_load = false;
	return ret;
}

bool Application::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.c_str());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	ModuleList::const_iterator item = modules.begin();

	for (; item != modules.end() && ret == true; item++)
	{
		pugi::xml_node node = root.append_child((*item)->name.c_str());
		ret = (*item)->Save(node);

	}

	if (ret == true)
	{
		data.save_file((char*)save_game.c_str(), "\t");
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (*item != nullptr) ? (*item)->name.c_str() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

// ---------------------------------------
int Application::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* Application::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* Application::GetTitle() const
{
	return title.c_str();
}


