#ifndef __ModuleSceneManager_H__
#define __ModuleSceneManager_H__

#include "Module.h"

class Scene;
enum SCENE_TYPE;

class ModuleSceneManager :	public Module
{
public:
	ModuleSceneManager();
	~ModuleSceneManager();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);
	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool PreUpdate();
	// Called each loop iteration
	virtual bool Update(float dt);
	// Called each loop iteration
	virtual bool PostUpdate();
	// Called before quitting
	virtual bool CleanUp();
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

	void ChangeScene(SCENE_TYPE _type);
	Scene* current_scene = nullptr;

	bool music_created = false;

private:
	pugi::xml_node scene_configs;

};

#endif // !__ModuleSceneManager_H_