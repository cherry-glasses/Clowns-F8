#ifndef __ModuleEntityManager_H__
#define __ModuleEntityManager_H__

#include "Module.h"

enum Movement_Type {
	PAWN,
	TOWER,
	KING,
	QUEEN,
	HORSE,
	BISHOP
};
//Cere Hemos de crear una enumaricón (jeje soy mongolo m'he he equivocado y me ha mucha gracia <3) de types mas larga

enum Type {
	ENEMY,
	CHARACTER,
	BOSS

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
	//bool CreateEntity(ENTITY_TYPE type);
	//bool DeleteEntity(Entity* entity);

private:
	//std::list<Entity*> entities;
};
#endif // __ModuleEntityManager_H__
