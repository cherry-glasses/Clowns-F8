#ifndef __ModuleEntityManager_H__
#define __ModuleEntityManager_H__

#include "Module.h"
#include "Entity.h"


class ModuleEntityManager : public Module
{
public:
	ModuleEntityManager();

	// Destructor
	virtual ~ModuleEntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float _dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Entity Manager
	Entity* CreateEntity(ENTITY_TYPE type);
	bool DeleteEntity(Entity* entity);
	std::pair<int,int> NearestCharacter(std::pair<int, int> myposition);

	bool UpdateWalk(std::pair<int, int> tile_id);

	void ThrowAttack(std::vector<std::pair<int,int>> _positions, int _damage, ENTITY_TYPE _type);
	std::list<Entity*> characters;

private:
	std::list<Entity*> entities;
	std::list<Entity*> enemies;
	pugi::xml_node entity_configs;

	bool starting = true;
	
};
#endif // !__ModuleEntityManager_H__
