#ifndef __ChooseMap_H_
#define __ChooseMap_H_

#include "Scene.h"

struct SDL_Texture;

class ChooseMap : public Scene
{
public:
	ChooseMap(SCENE_TYPE _type, pugi::xml_node& _config);
	~ChooseMap();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Load and Save
	virtual bool Load(pugi::xml_node& _data);
	virtual bool Save(pugi::xml_node& _data) const;

private:

	void CreateMenu();

private:

	SDL_Texture * menu_background = nullptr;
	SDL_Texture * maps_texture = nullptr;
	GUILabel* map1_label = nullptr;
	GUILabel* map2_label = nullptr;
	GUILabel* map3_label = nullptr;
	GUILabel* map4_label = nullptr;
	SDL_Rect alpha;
	SDL_Rect map1, map2, map3, map4;
	std::pair<int, int> UI_maps_size;
	int margin;
	int map_selected;
	
};

#endif //!__ChooseMap_H_