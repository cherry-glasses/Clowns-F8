#ifndef __ControlsMenu_H_
#define __ControlsMenu_H_

#include "Scene.h"

struct SDL_Texture;

class ControlsMenu : public Scene
{
public:
	ControlsMenu(SCENE_TYPE _type, pugi::xml_node& _config);
	~ControlsMenu();

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
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

private:
	void CreateControlsMenu();

private:
	SDL_Texture * main_menu_background = nullptr;
	SDL_Texture* controls_menu_background = nullptr;

	//Controls menu options UI
	GUIButton* accept_button = nullptr;
	GUIButton* decline_button = nullptr;
	GUIButton* character_stats_button = nullptr;
	GUIButton* characeter_abilities_button = nullptr;
	GUIButton* abilities_button = nullptr;
	GUIButton* start_button = nullptr;
	GUIButton* select_button = nullptr;
	GUIButton* back_button = nullptr;

	GUILabel* accept_label = nullptr;
	GUILabel* decline_label = nullptr;
	GUILabel* character_stats_label = nullptr;
	GUILabel* character_abilites_label = nullptr;
	GUILabel* abilities_label = nullptr;
	GUILabel* start_label = nullptr;
	GUILabel* select_label = nullptr;
	GUILabel* back_label = nullptr;

	bool controls_menu_created = false;

};

#endif //!__ControlsMenu_H_