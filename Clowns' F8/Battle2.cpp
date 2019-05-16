#include "Application.h"
#include "Battle2.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleMap.h"
#include "ModuleEntityManager.h"


Battle2::Battle2(SCENE_TYPE _type, pugi::xml_node& _config) : Battle(_type, _config)
{

}

// Destructor
Battle2::~Battle2()
{}


// Called before the first frame
bool Battle2::Start()
{
	battle_background = App->textures->Load("Assets/Maps/map_level_2.png");
	battle_grid = App->textures->Load("Assets/Maps/grid_level_2.png");
	battle_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->PlayMusic("Battle_2_Song.ogg");
	CreateBattle2();

	return true;
}

// Called before all Updates
bool Battle2::PreUpdate()
{
	return true;
}

// Called before all Updates
bool Battle2::PostUpdate(float dt)
{
	return true;
}

// Load
bool Battle2::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool Battle2::Save(pugi::xml_node& _data) const
{
	return true;
}

void Battle2::CreateBattle2()
{
	App->map->Load("map_level2.tmx");
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_STORM);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_BURGDOG);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_HOTDOG);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_STONE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_VOLCANO);

	CreateUIBattle();

}




