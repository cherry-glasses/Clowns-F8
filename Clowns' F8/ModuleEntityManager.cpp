#include "Log.h"
#include "Application.h"
#include "ModuleEntityManager.h"

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
	return true;
}

// Called each loop iteration
bool ModuleEntityManager::PreUpdate()
{
	/*for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->PreUpdate();
	}*/

	return true;
}

bool ModuleEntityManager::Update(float _dt)
{
	/*for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->Update(_dt);
	}*/

	return true;
}



bool ModuleEntityManager::PostUpdate()
{
	/*for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->PostUpdate();
	}*/

	return true;
}

// Called before quitting
bool ModuleEntityManager::CleanUp()
{
	/*for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->CleanUp();
		entity = entities.erase(entity);
	}
	entities.clear();*/

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

//bool ModuleEntityManager::CreateEntity(ENTITY_TYPE _type)
//{
//	std::string id = std::to_string(id_count);
//	switch (type)
//	{
//	case EntityType::TROOP:
//	{
//		id += "_" + card->name;
//		DynamicEntity* entity = new DynamicEntity();
//		entity->SetCard(card);
//		entities.push_back(entity);
//	}
//	break;
//	case EntityType::BUILDING:
//		//TODO
//		break;
//	}
//
//	id_count++;
//
//	return true;
//}
//
//bool ModuleEntityManager::DeleteEntity(Entity * entity)
//{
//	entities.remove(entity);
//	return true;
//}
