#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include <vector>

class GUIImage;
class GUIButton;
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

	void SelectUp();
	void SelectDown();
	void Select();

private:
	SDL_Texture* main_menu_background = nullptr;
	enum SCENES { MAIN_MENU, GLOBAL_MAP, FIRST_BATTLE };

	SCENES current_scene = MAIN_MENU;

public:

	bool map_loaded = false;
	GUIImage* cherry_glasses_logo = nullptr;
	GUIButton* new_game_button = nullptr;
	GUIButton* load_game_button = nullptr;
	GUIButton* options_button = nullptr;
	GUIButton* credits_button = nullptr;
	GUIButton* exit_button = nullptr;

	GUIButton* selected_button = nullptr;
	std::vector<GUIButton*> buttons;

	bool exitpressed = false;


	int key_select = -1;

};

#endif //_SCENE_H_