#include "Log.h"
#include "Application.h"
#include "ModuleEntityManager.h"
#include "Enemy.h"
#include "Character.h"
#include "CharacterIris.h"
#include "Hotdog.h"


ModuleEntityManager::ModuleEntityManager() : Module()
{
	name = "entity_manager";
}

// Destructor
ModuleEntityManager::~ModuleEntityManager()
{}

// Called before render is available
bool ModuleEntityManager::Awake(pugi::xml_node & _config)
{
	LOG("Loading Entities");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ModuleEntityManager::Start()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->Start();
	}
	return true;
}

// Called each loop iteration
bool ModuleEntityManager::PreUpdate()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->PreUpdate();
	}

	return true;
}

bool ModuleEntityManager::Update(float _dt)
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->Update(_dt);
	}

	return true;
}



bool ModuleEntityManager::PostUpdate()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->PostUpdate();
	}

	return true;
}

// Called before quitting
bool ModuleEntityManager::CleanUp()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->CleanUp();
		entity = entities.erase(entity);
	}
	entities.clear();

	return true;
}

// Load Game State
bool ModuleEntityManager::Load(pugi::xml_node & _data)
{
	return true;
}

// Save Game State
bool ModuleEntityManager::Save(pugi::xml_node & _data) const
{
	return true;
}

bool ModuleEntityManager::CreateEntity(ENTITY_TYPE _type)
{
	Entity* tmp = nullptr;

	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
		tmp = new CharacterIris(_type);
		entities.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_HOTDOG:
		tmp = new Hotdog(_type);
		entities.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BURGDOG:
		tmp = new CharacterIris(_type);
		entities.push_back(tmp);
		break;
	default:
		break;
	}

	return true;
}

bool ModuleEntityManager::DeleteEntity(Entity * entity)
{
	entities.remove(entity);
	return true;
}
