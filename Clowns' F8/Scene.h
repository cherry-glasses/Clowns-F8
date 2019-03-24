#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

class GUIImage;
class GUIButton;
class GUILabel;
struct SDL_Texture;

class Scene : public Module {


public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load
	bool Load(pugi::xml_node& savegame);

	//Save
	bool Save(pugi::xml_node& data) const;

private:
	void CreateMainMenu();
	void DeleteMainMenu();

	void NavigateDown();
	void NavigateUp();

private:
	SDL_Texture* main_menu_background = nullptr;
	SDL_Texture* health_bar_tex = nullptr;
	SDL_Texture* mana_bar_tex = nullptr;
	SDL_Texture* portrait_tex = nullptr;
	SDL_Texture* options_background = nullptr;

	enum SCENES { MAIN_MENU, GLOBAL_MAP, FIRST_BATTLE, MM_OPTIONS };

	SCENES current_scene = MAIN_MENU;

	bool main_menu_created = false;
	bool map_loaded = false;

	GUIButton* cherry_glasses_logo = nullptr;
	GUIButton* new_game_button = nullptr;
	GUIButton* load_game_button = nullptr;
	GUIButton* options_button = nullptr;
	GUIButton* credits_button = nullptr;
	GUIButton* exit_button = nullptr;
	GUILabel* label_test = nullptr;

	std::list<GUIButton*> buttons;

};

#endif //_SCENE_H_