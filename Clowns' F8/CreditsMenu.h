#ifndef __CreditsMenu_H_
#define __CreditsMenu_H_

#include "Scene.h"

struct SDL_Texture;

class CreditsMenu : public Scene
{
public:
	CreditsMenu(SCENE_TYPE _type, pugi::xml_node& _config);
	~CreditsMenu();

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
	void CreateCreditsMenu();

private:
	SDL_Texture * main_menu_background = nullptr;
	SDL_Texture* credits_menu_background = nullptr;

	GUIButton* back_button = nullptr;

	GUILabel* back_label = nullptr;

	bool credits_menu_created = false;

};

#endif //!__CreditsMenu_H_