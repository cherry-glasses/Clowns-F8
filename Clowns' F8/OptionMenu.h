#ifndef __OptionMenu_H_
#define __OptionMenu_H_

#include "Scene.h"

struct SDL_Texture;

class OptionMenu : public Scene
{
public:
	OptionMenu(SCENE_TYPE _type, pugi::xml_node& _config);
	~OptionMenu();

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
	void CreateOptionMenu();
	void DeleteOptionMenu();
	void ControlLanguageAndMusic();

private:
	SDL_Texture * main_menu_background = nullptr;
	SDL_Texture* option_menu_background = nullptr;

	
	GUIButton* english_button = nullptr;
	GUIButton* spanish_button = nullptr;
	GUIButton* volume_up_button = nullptr;
	GUIButton* volume_down_button = nullptr;
	GUIButton* back_button = nullptr;
	GUIButton* mm_button = nullptr;
	GUIButton* controls_button = nullptr;

	GUILabel* english_label = nullptr;
	GUILabel* spanish_label = nullptr;
	GUILabel* volume_up_label = nullptr;
	GUILabel* volume_down_label = nullptr;
	GUILabel* back_label = nullptr;
	GUILabel* controls_label = nullptr;
	GUILabel* language_label = nullptr;
	GUILabel* volume_label = nullptr;
	GUILabel* resume_label = nullptr;
	GUILabel* mm_label = nullptr;

	bool option_menu_created = false;

};

#endif //!__OptionMenu_H_