#ifndef __Lose_H_
#define __Lose_H_

#include "Scene.h"

struct SDL_Texture;

class Lose : public Scene
{
public:
	Lose(SCENE_TYPE _type, pugi::xml_node& _config);
	~Lose();

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

	void CreateLose();

private:

	SDL_Texture * lose_background = nullptr;
	GUILabel* lose_label = nullptr;

};

#endif //!__Lose_H_