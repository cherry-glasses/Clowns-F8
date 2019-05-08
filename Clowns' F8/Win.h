#ifndef __Win_H_
#define __Win_H_

#include "Scene.h"

struct SDL_Texture;

class Win : public Scene
{
public:
	Win(SCENE_TYPE _type, pugi::xml_node& _config);
	~Win();

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

	void CreateWin();

private:

	SDL_Texture * win_background = nullptr;
	GUILabel* win_label = nullptr;

};

#endif //!__Win_H_