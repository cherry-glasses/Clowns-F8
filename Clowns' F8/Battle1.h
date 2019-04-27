#ifndef __Battle1_H_
#define __Battle1_H_

#include "Battle.h"

struct SDL_Texture;

class Battle1 : public Battle
{
public:
	Battle1(SCENE_TYPE _type, pugi::xml_node& _config);
	~Battle1();

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
	void CreateMusic();
	void CreateBattle1();

private:

	SDL_Texture* battle1_background = nullptr;
	SDL_Texture* battle1_grid = nullptr;


	bool battle1_created = false;

};

#endif //!__Battle1_H_