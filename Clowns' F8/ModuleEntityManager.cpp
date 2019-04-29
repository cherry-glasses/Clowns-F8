#include "Log.h"
#include "Application.h"
#include "ModuleEntityManager.h"
#include "Enemy.h"
#include "Character.h"
#include "Object.h"
#include "CharacterSapphire.h"
#include "CharacterIris.h"
#include "CharacterStorm.h"
#include "CharacterGeorgeB.h"
#include "Boneyman.h"
#include "Pinkking.h"
#include "Hotdog.h"
#include "Tree1.h"
#include "Tree2.h"
#include "Tree3.h"
#include "BearTrap.h"
#include "ModulePathfinding.h"
#include "ModuleMap.h"
#include "Scene.h"
#include "Entity.h"


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


// Called each loop iteration
bool ModuleEntityManager::PreUpdate()
{
	if (entities.size() > 1) 
	{
		OrderEntitiesByAgility();
		if (starting) 
		{
			Entity *entityfirst = entities.front();
			entityfirst->current_turn = Entity::TURN::SEARCH_MOVE;
			starting = false;
		}
	}
	for (std::list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
	{
		if ((*enemy)->current_state == Entity::STATE::DEATH) {
			App->entity_manager->DeleteEntity((*enemy));
			break;
		}
	}
	for (std::list<Entity*>::iterator object = objects.begin(); object != objects.end(); ++object)
	{
		if (((Object*)*object)->used == true) {
			App->entity_manager->DeleteEntity((*object));
			break;
		}
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
			}
			else {
				entities.front()->current_turn = Entity::TURN::SEARCH_MOVE;
			}
		}
		if ((*entity)->stunned == true && (*entity)->current_turn == Entity::TURN::SEARCH_MOVE) {
			(*entity)->current_turn = Entity::TURN::END_TURN;
			(*entity)->stunned = false;
		}
		if ((*entity)->current_turn == Entity::TURN::SEARCH_MOVE) {
			// Starting Turn
			(*entity)->current_stats.Mana += 10;
			(*entity)->defend = false;

			// BearTrap
			if (std::find(enemies.begin(), enemies.end(), (*entity)) != enemies.end()) {
				for (std::list<Entity*>::iterator object = objects.begin(); object != objects.end(); ++object) {
					if ((*object)->GetPosition() == (*entity)->GetPosition()) {
						(*entity)->current_stats.Hp -= (george_b->current_stats.AtkS - (george_b->current_stats.AtkS * (*entity)->current_stats.DefS / 100));
						((Object*)*object)->used = true;
					}
				}
			}
		}
		(*entity)->PreUpdate();
		
	}

	int w, h;
	uchar* dat = NULL;

	if (App->map->CreateWalkabilityMap(w, h, &dat))
		App->pathfinding->SetMap(w, h, dat);
	return true;
}

bool ModuleEntityManager::Update(float _dt)
{
	if (!paused) {
		for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
		{
			(*entity)->Update(_dt);
		}
	}
	
	return true;
}


bool ModuleEntityManager::PostUpdate()
{
	if (!paused) {
		if (entities.size() > 1)
		{
			OrderEntitiesByPosition();
		}
		for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
		{
			if ((*entity)->current_stats.Hp > (*entity)->default_stats.Hp) {
				(*entity)->current_stats.Hp = (*entity)->default_stats.Hp;
			}
			else if ((*entity)->current_stats.Hp < 0) {
				(*entity)->current_stats.Hp = 0;
			}
			if ((*entity)->current_stats.Mana >(*entity)->default_stats.Mana) {
				(*entity)->current_stats.Mana = (*entity)->default_stats.Mana;
			}
			else if ((*entity)->current_stats.Mana < 0) {
				(*entity)->current_stats.Mana = 0;
			}
			(*entity)->PostUpdate();

		}

	}
	return true;
}

// Called before quitting
bool ModuleEntityManager::CleanUp()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->CleanUp();
	}
	entities.clear();
	characters.clear();
	enemies.clear();
	objects.clear();
	paused = false;
	starting = true;

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

bool CompareByAgility(Entity* first, Entity* second) {
	return (first->current_stats.Agi > second->current_stats.Agi);
}
bool CompareByPosition(Entity* first, Entity* second) {
	return (first->GetPosition().second < second->GetPosition().second);
}

void ModuleEntityManager::OrderEntitiesByAgility() {
	entities.sort(CompareByAgility);
	characters.sort(CompareByAgility);
	enemies.sort(CompareByAgility);
	objects.sort(CompareByAgility);
}
void ModuleEntityManager::OrderEntitiesByPosition() {
	entities.sort(CompareByPosition);
	characters.sort(CompareByPosition);
	enemies.sort(CompareByPosition);
	objects.sort(CompareByPosition);
}

std::pair<int, int> ModuleEntityManager::NearestCharacter(std::pair<int, int> myposition){
	std::pair<int, int> tmp;
	std::pair<int, int> tmp_allied;
	int tmp_result;
	int tmp_result_2 = 30000;

	for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
		tmp_allied = (*character)->GetPosition();
		tmp_result = sqrt(((tmp_allied.first - myposition.first)*(tmp_allied.first - myposition.first)) + ((tmp_allied.second - myposition.second)*(tmp_allied.second - myposition.second)));
		if (tmp_result < tmp_result_2 && (*character)->current_state ==  (*character)->ALIVE) {
			tmp = tmp_allied;
			tmp_result_2 = tmp_result;
		}
			
	}
	return tmp;

}

std::pair<int, int> ModuleEntityManager::CharactersPrioritzationAttack(std::pair<int, int>* AttackRange, int AttackRangeint)
{

	Entity* Charrr = nullptr;

	for (int i = 0; i < AttackRangeint; i++) {
		std::pair<int, int> pos = App->map->MapToWorld(AttackRange[i].first, AttackRange[i].second);
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
			if (pos == (*character)->GetPosition()) {
				if (Charrr == nullptr) {
					Charrr = (*character);
				}
				else {
					if ((*Charrr).current_stats.DefF <= (*character)->current_stats.DefF) {
						Charrr = (*character);
					}
				}
			}
			

		}

	}

	return (*Charrr).GetPosition();
}

Entity* ModuleEntityManager::CreateEntity(ENTITY_TYPE _type)
{
	Entity* tmp = nullptr;

	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
		tmp = new CharacterSapphire(_type, entity_configs.child("sapphire"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
		tmp = new CharacterIris(_type, entity_configs.child("iris"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
		tmp = new CharacterStorm(_type, entity_configs.child("storm"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
		tmp = new CharacterGeorgeB(_type, entity_configs.child("georgeb"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		george_b = (CharacterGeorgeB*) tmp;
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN:
		for (int i = 0; i < 4; i++)
		{
			tmp = new Boneyman(_type, entity_configs.child("boneyman"), i);
			entities.push_back(tmp);
			enemies.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_PINKKING:
		tmp = new Pinkking(_type, entity_configs.child("pinkking"));
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
	case ENTITY_TYPE::ENTITY_OBJECT_TREE1:
		for (int i = 0; i < 5; i++)
		{
			tmp = new Tree1(_type, entity_configs.child("tree1"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_TREE2:
		for (int i = 0; i < 7; i++)
		{
			tmp = new Tree2(_type, entity_configs.child("tree2"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_TREE3:
		for (int i = 0; i < 15; i++)
		{
			tmp = new Tree3(_type, entity_configs.child("tree3"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_BEARTRAP:
		tmp = new BearTrap(_type, entity_configs.child("beartrap"));
		entities.push_back(tmp);
		objects.push_back(tmp);
		break;
	default:
		break;
	}

	return tmp;
}

bool ModuleEntityManager::DeleteEntity(Entity * entity)
{
	if (std::find(enemies.begin(), enemies.end(), (entity)) != enemies.end()) {
		enemies.remove(entity);
	}
	else if(std::find(objects.begin(), objects.end(), (entity)) != objects.end()) {
		objects.remove(entity);
	}
	entities.remove(entity);
	entity->CleanUp();
	entity = nullptr;
	
	return true;
}



void ModuleEntityManager::ThrowAttack(std::vector<std::pair<int, int>> _positions, int _damage, ENTITY_TYPE _type)
{
	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
		{
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				if ((*character)->GetPosition() == (*position))
				{
					(*character)->current_stats.Hp += _damage;
				}
			}
		}
		for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
		{
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				if ((*enemie)->GetPosition() == (*position))
				{
					(*enemie)->current_stats.Hp -= (_damage - (_damage * (*enemie)->current_stats.DefS / 100));
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
		for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
		{
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				if ((*enemie)->GetPosition() == (*position))
				{
					(*enemie)->current_stats.Hp -= (_damage - (_damage * (*enemie)->current_stats.DefF / 100));
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
		for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
		{
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				if ((*enemie)->GetPosition() == (*position))
				{
					if (_damage == 0) {
						(*enemie)->stunned = true;
					}
					else {
						(*enemie)->current_stats.Hp -= (_damage - (_damage * (*enemie)->current_stats.DefF / 100));
					}
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
		if (_damage == 0) {
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				BearTrap *tmp = (BearTrap*)CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_BEARTRAP);
				(*tmp).SetPosition((*position).first, (*position).second);
			}
		}
		else {
			for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*enemie)->GetPosition() == (*position))
					{
						(*enemie)->current_stats.Hp -= (_damage - (_damage * (*enemie)->current_stats.DefS / 100));
					}
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_PINKKING:
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
		{
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				if ((*character)->GetPosition() == (*position))
				{
					if ((*character)->defend) {
						(*character)->current_stats.Hp -= (_damage - (_damage * (*character)->current_stats.DefF * 1.2 / 100));
					}
					else {
						(*character)->current_stats.Hp -= (_damage - (_damage * (*character)->current_stats.DefF / 100));
					}

				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN:
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
		{
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				if ((*character)->GetPosition() == (*position))
				{
					if ((*character)->defend) {
						(*character)->current_stats.Hp -= (_damage - (_damage * (*character)->current_stats.DefF * 1.2 / 100));
					}
					else {
						(*character)->current_stats.Hp -= (_damage - (_damage * (*character)->current_stats.DefF / 100));
					}
					
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_HOTDOG:
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BURGDOG:
		break;
	case ENTITY_TYPE::NO_TYPE:
		break;
	default:
		break;
	}
	
}


bool ModuleEntityManager::UpdateWalk(std::pair<int, int> tile_id) {
	bool ret = false;
	std::pair<int, int> tmp;
	for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie) {
		tmp = (*enemie)->GetPosition();
		tmp = App->map->WorldToMap(tmp.first, tmp.second);
		if (tile_id == tmp) {
			ret = true;
		}
	}
	return ret;
}

bool ModuleEntityManager::ThereAreCharAlive()
{
	bool flag = false;
	for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
		if ((*character)->current_state == (*character)->ALIVE) {
			flag = true;
			break;
		}
	}
	return flag;
}



std::pair<int, int>* ModuleEntityManager::RangeOfAttack(std::pair<int, int> myposition, int radius, int& size) {

	std::list<std::pair<int, int>> frontier;
	std::list<std::pair<int, int>> visited;
	bool flag = false;

	std::pair<int, int> aux;
	// calcular con los 2 radios y restar.
	frontier.push_back(myposition);
	//visited.push_back(myposition);
	while (frontier.size() != 0) {
		aux = frontier.front();
		frontier.pop_front();
		flag = false;
		for (std::list<std::pair<int, int>>::iterator eshorrible = visited.begin(); eshorrible != visited.end(); ++eshorrible) {
			if (aux == (*eshorrible)) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			for (int i = -1; i <= 1; i += 2) {
				for (int j = -1; j <= 1; j += 2) {
					std::pair<int, int> tmp = aux;
					if (i == -1)
						tmp.first += j;
					else 
						tmp.second += j;
					
					if (InRange(myposition, tmp, radius)) {
						frontier.push_back(tmp);
					}
				}
			}
			visited.push_back(aux);
		}
	}

	size = visited.size();
	std::pair<int, int>* ret = new std::pair<int,int>[visited.size()];
	int i = 0;
	for (std::list<std::pair<int, int>>::iterator eshorrible = visited.begin(); eshorrible != visited.end(); ++eshorrible, i++) {
		ret[i] = (*eshorrible);
	}

	return ret;
}

