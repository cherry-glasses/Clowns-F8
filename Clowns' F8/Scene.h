#ifndef __Scene_H__
#define __Scene_H__

#include "GUIButton.h"
#include "GUILabel.h"
#include "GUIImage.h"
#include "ModuleSceneManager.h"


enum SCENE_TYPE {
	MAIN_MENU,
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
	virtual bool PostUpdate(float dt) { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&) { return true; };

	virtual bool Save(pugi::xml_node&) const { return true; };


protected:
	
	// Navigation
	void Navigate();
	void NavigateDown();
	void NavigateUp();

public:
	SCENE_TYPE type = SCENE_TYPE::NONE;

protected:
	std::list<GUIButton*> buttons;

	SDL_Rect button;
	SDL_Rect small_button;
	SDL_Rect cherry_glasses_logo;
	SDL_Rect option_background;

	int button_margin;

	int screen_width, screen_height;
	
	// Audios
	int	press_sfx;
	int	defend_sfx;
	int	critic_sfx;

};

#endif //!__Scene_H_