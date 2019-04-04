#include "Log.h"
#include "Application.h"
#include "ModuleEntityManager.h"
#include "Enemy.h"
#include "Character.h"
#include "CharacterIris.h"
#include "Boneyman.h"
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
	entity_configs = _config;
	return true;
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

bool CompareByPosition(Entity* first, Entity* second) {
	return (first->GetPosition().second < second->GetPosition().second);
}

bool CompareByAgility(Entity* first, Entity* second) {
	return (first->current_stats.Agi > second->current_stats.Agi);
}

// Called each loop iteration
bool ModuleEntityManager::PreUpdate()
{
	if (entities.size() > 1) 
	{
		entities.sort(CompareByAgility);
		if (starting) 
		{
			std::list<Entity*>::iterator entityfirst = entities.begin();
			(*entityfirst)->current_turn = Entity::TURN::SEARCH_MOVE;
			starting = false;
		}
	}
	else
	{
		starting = true;
	}
	
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if ((*entity)->current_turn == Entity::TURN::END_TURN)
		{
			(*entity)->current_turn = Entity::TURN::NONE;
			if (*entity != entities.back())
			{
				entity++;
				(*entity)->current_turn = Entity::TURN::SEARCH_MOVE;
				entity--;
			}
			else {
				entities.front()->current_turn = Entity::TURN::SEARCH_MOVE;
			}

		}

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
	if (entities.size() > 1)
	{
		entities.sort(CompareByPosition);
	}
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

std::pair<int, int> ModuleEntityManager::NearestCharacter(std::pair<int, int> myposition){
	std::pair<int, int> tmp;
	std::pair<int, int> tmp_allied;
	int tmp_result;
	int tmp_result_2 = 30000;

	for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
		tmp_allied = (*character)->GetPosition();
		tmp_result = sqrt(((tmp_allied.first - myposition.first)*(tmp_allied.first - myposition.first)) + ((tmp_allied.second - myposition.second)*(tmp_allied.second - myposition.second)));
		if (tmp_result < tmp_result_2)
			tmp = tmp_allied;
	}
	return tmp;

}

bool ModuleEntityManager::CreateEntity(ENTITY_TYPE _type)
{
	Entity* tmp = nullptr;

	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
		tmp = new CharacterIris(_type, entity_configs.child("iris"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN:
		tmp = new Boneyman(_type, entity_configs.child("boneyman"), enemies.size());
		entities.push_back(tmp);
		enemies.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_HOTDOG:
		tmp = new Hotdog(_type, entity_configs.child("hotdog"));
		entities.push_back(tmp);
		enemies.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BURGDOG:
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



void ModuleEntityManager::ThrowAttack(std::vector<std::pair<int, int>> _positions, int _damage, ENTITY_TYPE _type)
{
	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_HOTDOG:
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
		{
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position) 
			{
				if ((*character)->GetPosition() == (*position)) 
				{
					(*character)->current_stats.Hp -= (_damage - (*character)->current_stats.DefF);
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BURGDOG:
		break;
	case ENTITY_TYPE::NO_TYPE:
		break;
	default:
		break;
	}
	
}


