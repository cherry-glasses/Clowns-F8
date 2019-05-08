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

	// Called before all Updates
	bool PostUpdate();

	// Load and Save
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

private:

	void CreateBattle1();

};

#endif //!__Battle1_H_