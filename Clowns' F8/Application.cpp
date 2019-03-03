#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleMap.h"
#include "ModuleEntityManager.h"
#include "Scene.h"
#include "ModuleGUIManager.h"


Application::Application(int argc, char* args[]) : argc(argc), args(args)
{
	window = new ModuleWindow();
	render = new ModuleRender();
	input = new ModuleInput();
	textures = new ModuleTextures();
	audio = new ModuleAudio();
	fade_to_black = new ModuleFadeToBlack();
	fonts = new ModuleFonts();
	scene = new Scene();
	map = new ModuleMap();
	entity_manager = new ModuleEntityManager();
	gui_manager = new ModuleGUIManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(window);
	AddModule(input);
	AddModule(textures);
	AddModule(audio);
	AddModule(fade_to_black);
	AddModule(fonts);
	AddModule(scene);
	AddModule(map);
	AddModule(entity_manager);
	AddModule(gui_manager);

	// render last to swap buffer
	AddModule(render);

}

Application::~Application()
{
	for (ModuleList::reverse_iterator item = modules.rbegin(); item != modules.rend(); item++) {
		//Utils::Release(*item);
	}
	modules.clear();
}

bool Application::Awake()
{
	bool ret = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
		frame_rate = app_config.child("framerate_cap").attribute("rate").as_uint();
		framerate_cap_enabled = app_config.child("framerate_cap").attribute("enabled").as_bool();
		save_game.create(app_config.child("save").child_value());
		load_game = save_game;

	}

	if (ret == true)
	{
		p2List_item<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	for (int i = 0; i < NUM_MODULES && ret == true; ++i)
		ret = modules[i]->Init();

	for (int i = 0; i < NUM_MODULES && ret == true; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : UPDATE_CONTINUE;

	return ret;
}


bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret == true; --i)
		ret = modules[i]->CleanUp();

	return ret;
}

