#ifndef __Intro_H_
#define __Intro_H_

#include "Scene.h"

struct SDL_Texture;

class Intro : public Scene
{
public:
	Intro(SCENE_TYPE _type, pugi::xml_node& _config);
	~Intro();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Load and Save
	virtual bool Load(pugi::xml_node& _data);
	virtual bool Save(pugi::xml_node& _data) const;

private:

	bool lines_created = false;
	int which_text = 0;

private:

	std::vector<GUILabel*> labels;
};

#endif //!__Intro_H_