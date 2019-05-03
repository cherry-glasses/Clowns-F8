#ifndef __Scene_H__
#define __Scene_H__

#include "GUIButton.h"
#include "GUILabel.h"
#include "GUIImage.h"
#include "ModuleSceneManager.h"


enum SCENE_TYPE {
	MAIN_MENU,
	GLOBAL_MAP,
	FIRST_BATTLE,
	SECOND_BATTLE,
	THIRD_BATTLE,
	FOURTH_BATTLE,
	WIN_SCENE,
	LOSE_SCENE,
	CHOOSE_MAP,
	NONE
};

class Scene 
{
public:

	Scene(SCENE_TYPE _type, pugi::xml_node& _config);

	// Destructor
	virtual ~Scene();

	// Called before the first frame
	virtual bool Start() { return true; };

	// Called before all Updates
	virtual bool PreUpdate() { return true; };

	// Called each loop iteration
	virtual bool Update(float dt) { return true; };

	// Called before all Updates
	virtual bool PostUpdate() { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&) { return true; };

	virtual bool Save(pugi::xml_node&) const { return true; };


protected:
	
	// Navigation
	void Navigate();
	void NavigateDown();
	void NavigateUp();

	//Win and Lose
	/*void Win();
	void Lose();*/

public:
	SCENE_TYPE type = SCENE_TYPE::NONE;

protected:
	std::list<GUIButton*> buttons;

	SDL_Rect button;
	SDL_Rect small_button;
	SDL_Rect cherry_glasses_logo;
	SDL_Rect option_background;
	SDL_Rect iris_portrait;
	SDL_Rect sapphire_portrait;
	SDL_Rect george_b_portrait;
	SDL_Rect storm_portrait;
	std::pair<int, int> portrait_margin;
	std::pair<int, int> life_margin;
	std::pair<int, int> mana_margin;
	std::pair<int, int> port_margin;
	std::pair<int, int> name_margin;
	std::pair<int, int> actions_margin;
	
	

	int button_margin;

	int screen_width, screen_height;
	
	std::string	press_fx_name = "";

	

};

#endif //!__Scene_H_