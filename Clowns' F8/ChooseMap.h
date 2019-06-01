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

	void NavigateMaps();
	void CreateMenu();
	void DeleteLabels();

private:

	SDL_Texture * menu_background = nullptr;
	SDL_Texture * maps_texture = nullptr;
	GUILabel* map1_label = nullptr;
	GUILabel* map2_label = nullptr;
	GUILabel* map3_label = nullptr;
	GUILabel* map4_label = nullptr;
	SDL_Rect alpha;
	SDL_Rect map1, map2, map3, map4;
	int map_selected;
	bool debug_maps = false;
	bool debug_map_1 = false;
	bool debug_map_2 = false;
	bool debug_map_3 = false;

	GUIButton* main_menu_button = nullptr;
	GUIButton* save_game_button = nullptr;
	GUILabel* main_menu_label = nullptr;
	GUILabel* save_game_label = nullptr;
	GUILabel* saved_label = nullptr;

};

#endif //!__ChooseMap_H_