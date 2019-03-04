//#ifndef __SCENE_H__
//#define __SCENE_H__
//
//#include "Module.h"
//
//class Scene : public Module {
//
//
//public:
//
//
//	Scene();
//
//	// Destructor
//	virtual ~Scene();
//
//	// Called before render is available
//	bool Awake(pugi::xml_node& config);
//
//	// Called before the first frame
//	bool Start();
//
//	// Called before all Updates
//	bool PreUpdate();
//
//	// Called each loop iteration
//	bool Update(float dt);
//
//	// Called before all Updates
//	bool PostUpdate();
//
//	// Called before quitting
//	bool CleanUp();
//
//	// Load
//	bool Load(pugi::xml_node& savegame);
//
//	bool LoadScene(int map = -1);
//
//	//Save
//	bool Save(pugi::xml_node& data) const;
//
//};
//
//#endif //_SCENE_H_