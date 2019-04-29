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
	void CreateOptionMenu();
	void CreateControlsMenu();
	void CreateCreditsMenu();

	void Delete();

	void ControlLanguageAndMusic();

private:

	SDL_Texture* main_menu_background = nullptr;
	SDL_Texture* option_menu_background = nullptr;
	SDL_Texture* credits_menu_background = nullptr;

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

	// Option Menu UI
	GUIButton* english_button = nullptr;
	GUIButton* spanish_button = nullptr;
	GUIButton* volume_up_button = nullptr;
	GUIButton* volume_down_button = nullptr;
	GUIButton* back_option_button = nullptr;
	GUIButton* mm_button = nullptr;
	GUIButton* controls_button = nullptr;

	GUILabel* english_label = nullptr;
	GUILabel* spanish_label = nullptr;
	GUILabel* volume_up_label = nullptr;
	GUILabel* volume_down_label = nullptr;
	GUILabel* back_option_label = nullptr;
	GUILabel* controls_label = nullptr;
	GUILabel* language_label = nullptr;
	GUILabel* volume_label = nullptr;
	GUILabel* resume_label = nullptr;
	GUILabel* mm_label = nullptr;

	bool option_menu_created = false;

	//Controls menu options UI
	GUIButton* accept_button = nullptr;
	GUIButton* decline_button = nullptr;
	GUIButton* character_stats_button = nullptr;
	GUIButton* characeter_abilities_button = nullptr;
	GUIButton* abilities_button = nullptr;
	GUIButton* start_button = nullptr;
	GUIButton* select_button = nullptr;
	GUIButton* back_controls_button = nullptr;

	GUILabel* accept_label = nullptr;
	GUILabel* decline_label = nullptr;
	GUILabel* character_stats_label = nullptr;
	GUILabel* character_abilites_label = nullptr;
	GUILabel* abilities_label = nullptr;
	GUILabel* start_label = nullptr;
	GUILabel* select_label = nullptr;
	GUILabel* back_controls_label = nullptr;

	bool controls_menu_created = false;

	// Credits Menu UI
	GUIButton* back_credits_button = nullptr;

	GUILabel* back_credits_label = nullptr;

	bool credits_menu_created = false;





};

#endif //!__MainMenu_H_