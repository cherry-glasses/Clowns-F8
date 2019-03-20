#ifndef __ModuleEntityManager_H__
#define __ModuleEntityManager_H__

#include "Module.h"
#include "Entity.h"

enum class ENTITY_TYPE
{
	ENTITY_CHARACTER,
	ENTITY_CHARACTER_IRIS,
	ENTITY_ENEMY,
	ENTITY_BOSS,
	NO_TYPE
};

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

private:
	std::list<Entity*> entities;
};
#endif // !__ModuleEntityManager_H__
