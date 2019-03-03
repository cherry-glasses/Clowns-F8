#ifndef __ModuleEntityManager_H__
#define __ModuleEntityManager_H__

#include "Module.h"
#include "Globals.h"


class j1EntityManager : public Module
{
public:

	j1EntityManager();

	//bool Awake(pugi::xml_node &);
	bool Start();
	bool ReStart();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

private:
	//Entity containers
};

#endif // __ModuleEntityManager_H__
