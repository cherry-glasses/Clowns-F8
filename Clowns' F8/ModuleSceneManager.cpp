#include "Application.h"
#include "MainMenu.h"
#include "OptionMenu.h"
#include "ControlsMenu.h"
#include "CreditsMenu.h"
#include "Battle1.h"
#include "ModuleSceneManager.h"



ModuleSceneManager::ModuleSceneManager()
{
	name = "scene_manager";
}


ModuleSceneManager::~ModuleSceneManager()
{

}

bool ModuleSceneManager::Awake(pugi::xml_node & _config)
{
	scene_configs = _config;
	current_scene = new MainMenu(SCENE_TYPE::MAIN_MENU, _config);
	return true;
}

bool ModuleSceneManager::Start()
{
	current_scene->Start();
	return true;
}

bool ModuleSceneManager::PreUpdate()
{
	current_scene->PreUpdate();
	return true;
}

bool ModuleSceneManager::Update(float dt)
{
	current_scene->Update(dt);
	return true;
}

bool ModuleSceneManager::PostUpdate()
{
	current_scene->PostUpdate();
	return true;
}

bool ModuleSceneManager::CleanUp()
{
	current_scene->CleanUp();
	return true;
}

bool ModuleSceneManager::Load(pugi::xml_node &xml)
{
	current_scene->Load(xml);
	return true;
}

bool ModuleSceneManager::Save(pugi::xml_node &xml) const
{
	current_scene->Save(xml);
	return true;
}

//Scene unloads current scene, then creates and loads new scene.
void ModuleSceneManager::ChangeScene(SCENE_TYPE _type)
{
	current_scene->CleanUp();

	switch (_type)
	{
	case MAIN_MENU:
		current_scene = new MainMenu(_type, scene_configs);
		break;
	case GLOBAL_MAP:
		break;
	case OPTION_MENU:
		current_scene = new OptionMenu(_type, scene_configs);
		break;
	case CONTROLS_MENU:
		current_scene = new ControlsMenu(_type, scene_configs);
		break;
	case CREDITS_MENU:
		current_scene = new CreditsMenu(_type, scene_configs);
		break;
	case FIRST_BATTLE:
		current_scene = new Battle1(_type, scene_configs);
		break;
	case SECOND_BATTLE:
		break;
	case THIRD_BATTLE:
		break;
	case FOURTH_BATTLE:
		break;
	case WIN_SCENE:
		break;
	case LOSE_SCENE:
		break;
	case NONE:
		break;
	default:
		break;
	}

	current_scene->Start();
}