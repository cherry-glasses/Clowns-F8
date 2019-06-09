#include "Application.h"
#include "Battle1.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleMap.h"
#include "ModuleEntityManager.h"


Battle1::Battle1(SCENE_TYPE _type, pugi::xml_node& _config) : Battle(_type, _config)
{

}

// Destructor
Battle1::~Battle1()
{}


// Called before the first frame
bool Battle1::Start()
{
	battle_background = App->textures->Load("Assets/Maps/map_level_1.png");
	battle_grid = App->textures->Load("Assets/Maps/grid_level_1.png");
	battle_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");
	Tutorial_background = App->textures->Load("Assets/Sprites/UI/Tutorial_image.png");

	App->audio->PlayMusic("Battle_1_Song.ogg");
	CreateBattle1();

	return true;
}

// Called before all Updates
bool Battle1::PreUpdate()
{
	return true;
}

// Called before all Updates
bool Battle1::PostUpdate(float dt)
{
	return true;
}


// Load
bool Battle1::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool Battle1::Save(pugi::xml_node& _data) const
{
	return true;
}

void Battle1::CreateBattle1()
{
	App->map->Load("map_level1.tmx");
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_STORM);
	//*App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
	//App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_HOTDOG);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_CHERRYBLACKGLASSES);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_TREE1);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_TREE2);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_TREE3);
	App->entity_manager->LevelUP(0);
	CreateUIBattle();
	

}




