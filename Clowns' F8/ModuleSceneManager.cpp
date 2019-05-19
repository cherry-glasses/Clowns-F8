#include "Application.h"
#include "ModuleSceneManager.h"
#include "MainMenu.h"
#include "Battle1.h"
#include "Battle2.h"
#include "Battle3.h"
#include "Battle4.h"
#include "Win.h"
#include "Lose.h"
#include "ChooseMap.h"
#include "Language.h"


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
	language = new Language();
	return current_scene->Start();
}

bool ModuleSceneManager::PreUpdate()
{
	return current_scene->PreUpdate();
}

bool ModuleSceneManager::Update(float dt)
{
	return current_scene->Update(dt);
}

bool ModuleSceneManager::PostUpdate(float _dt)
{
	return current_scene->PostUpdate(_dt);
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
	
	delete current_scene;
	current_scene = nullptr;

	switch (_type)
	{
	case MAIN_MENU:
		current_scene = new MainMenu(_type, scene_configs);
		break;
	case FIRST_BATTLE:
		current_scene = new Battle1(_type, scene_configs);
		break;
	case SECOND_BATTLE:
		current_scene = new Battle2(_type, scene_configs);
		break;
	case THIRD_BATTLE:
		current_scene = new Battle3(_type, scene_configs);
		break;
	case FOURTH_BATTLE:
		current_scene = new Battle4(_type, scene_configs);
		break;
	case WIN_SCENE:
		current_scene = new Win(_type, scene_configs);
		break;
	case LOSE_SCENE:
		current_scene = new Lose(_type, scene_configs);
		break;
	case CHOOSE_MAP:
		current_scene = new ChooseMap(_type, scene_configs);
		break;
	case NONE:
		break;
	default:
		break;
	}

	current_scene->Start();
	changing = false;
}