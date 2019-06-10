#include "Application.h"
#include "Battle4.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleMap.h"
#include "ModuleEntityManager.h"


Battle4::Battle4(SCENE_TYPE _type, pugi::xml_node& _config) : Battle(_type, _config)
{

}

// Destructor
Battle4::~Battle4()
{}


// Called before the first frame
bool Battle4::Start()
{
	battle_background = App->textures->Load("Assets/Maps/map_level_4.png");
	battle_grid = App->textures->Load("Assets/Maps/grid_level_4.png");
	battle_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");

	App->audio->PlayMusic("Battle_4_Song.ogg");
	CreateBattle4();

	return true;
}

// Called before all Updates
bool Battle4::PreUpdate()
{
	return true;
}

// Called before all Updates
bool Battle4::PostUpdate(float dt)
{
	return true;
}


// Load
bool Battle4::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool Battle4::Save(pugi::xml_node& _data) const
{
	return true;
}

void Battle4::CreateBattle4()
{
	first_level_tut = false;
	App->map->Load("map_level4.tmx");
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_STORM);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_HOTDOG);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_POLARPATH);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_CHERRYBLACKGLASSES);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_MOUNTAIN2);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_STONE2);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_STONE3);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_STALAGMITE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_BLOODSTREAM);
	App->entity_manager->LevelUP(0);
	CreateUIBattle();
	
}




