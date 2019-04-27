#ifndef __MainMenu_H_
#define __MainMenu_H_

#include "Scene.h"

struct SDL_Texture;

class MainMenu : public Scene
{
public:
	MainMenu(SCENE_TYPE _type, pugi::xml_node& _config);
	~MainMenu();

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

	// Load and Save
	virtual bool Load(pugi::xml_node& _data);
	virtual bool Save(pugi::xml_node& _data) const;

private:
	void CreateMusic();
	void CreateMainMenu();

private:

	SDL_Texture * main_menu_background = nullptr;

	//Main menu UI
	GUIButton* cherry_glasses_logo_button = nullptr;

	GUIButton* new_game_button = nullptr;
	GUIButton* load_game_button = nullptr;
	GUIButton* options_button = nullptr;
	GUIButton* credits_button = nullptr;
	GUIButton* exit_button = nullptr;

	GUIImage* cherry_glasses_logo_image = nullptr;

	GUILabel* new_game_label = nullptr;
	GUILabel* load_game_label = nullptr;
	GUILabel* options_label = nullptr;
	GUILabel* credits_label = nullptr;
	GUILabel* exit_label = nullptr;

	bool main_menu_created = false;




};

#endif //!__MainMenu_H_