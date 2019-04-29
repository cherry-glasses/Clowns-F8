#include "Application.h"
#include "MainMenu.h"
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
	return current_scene->Start();;
}

bool ModuleSceneManager::PreUpdate()
{
	return current_scene->PreUpdate();;
}

bool ModuleSceneManager::Update(float dt)
{
	return current_scene->Update(dt);
}

bool ModuleSceneManager::PostUpdate()
{
	return current_scene->PostUpdate();
}

bool ModuleSceneManager::CleanUp()
{
	return current_scene->CleanUp();
}

bool ModuleSceneManager::Load(pugi::xml_node &xml)
{
	return current_scene->Load(xml);
}

bool ModuleSceneManager::Save(pugi::xml_node &xml) const
{
	return current_scene->Save(xml);
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