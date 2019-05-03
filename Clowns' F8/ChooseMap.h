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
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load and Save
	virtual bool Load(pugi::xml_node& _data);
	virtual bool Save(pugi::xml_node& _data) const;

private:
	void CreateMusic();
	void Delete();
	void CreateMenu();
	void ChangeMapImage();

private:

	bool menu_created = false;
	SDL_Texture * menu_background = nullptr;
	GUIButton* map1_button = nullptr;
	GUIButton* map2_button = nullptr;
	GUIButton* map3_button = nullptr;
	GUIButton* back_button = nullptr;
	GUILabel* map1_label = nullptr;
	GUILabel* map2_label = nullptr;
	GUILabel* map3_label = nullptr;
	GUILabel* back_label = nullptr;
	GUIImage* map_image = nullptr;
	GUIImage* frame = nullptr;

};

#endif //!__ChooseMap_H_