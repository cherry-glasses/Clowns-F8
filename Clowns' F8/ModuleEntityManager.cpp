#include "Application.h"
#include "ModuleEntityManager.h"

ModuleEntityManager::ModuleEntityManager()
{
	
}

//bool EntityManager::Awake(pugi::xml_node & node)
//{
//	//Create player
//	
//
//	//Load entities info
//
//	return true;
//}
bool ModuleEntityManager::Start()
{

}


update_status ModuleEntityManager::PreUpdate()
{
	return update_status::UPDATE_CONTINUE;
}
update_status ModuleEntityManager::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}
update_status ModuleEntityManager::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}
bool ModuleEntityManager::CleanUp()
{

	return true;
}
