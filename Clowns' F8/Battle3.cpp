#include "Application.h"
#include "Battle3.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleMap.h"
#include "ModuleEntityManager.h"


Battle3::Battle3(SCENE_TYPE _type, pugi::xml_node& _config) : Battle(_type, _config)
{

}

// Destructor
Battle3::~Battle3()
{}


// Called before the first frame
bool Battle3::Start()
{
	battle_background = App->textures->Load("Assets/Maps/map_level_3.png");
	battle_grid = App->textures->Load("Assets/Maps/grid_level_3.png");
	battle_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->PlayMusic("Battle_3_Song.ogg");
	CreateBattle3();

	return true;
}

// Called before all Updates
bool Battle3::PreUpdate()
{
	return true;
}

// Called before all Updates
bool Battle3::PostUpdate()
{
	return true;
}


// Load
bool Battle3::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool Battle3::Save(pugi::xml_node& _data) const
{
	return true;
}

void Battle3::CreateBattle3()
{
	App->map->Load("map_level3.tmx");
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_STORM);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_POLARBEAR);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_POLARPATH);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_ICE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_MOUNTAIN);

	CreateUIBattle();

}




