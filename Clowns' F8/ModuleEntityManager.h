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
	bool CreateEntity(ENTITY_TYPE type);
	bool DeleteEntity(Entity* entity);

private:
	std::list<Entity*> entities;
	pugi::xml_node entity_configs;
	
};
#endif // !__ModuleEntityManager_H__
