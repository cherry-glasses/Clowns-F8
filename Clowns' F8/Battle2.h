#ifndef __Battle2_H_
#define __Battle2_H_

#include "Battle.h"

struct SDL_Texture;

class Battle2 : public Battle
{
public:
	Battle2(SCENE_TYPE _type, pugi::xml_node& _config);
	~Battle2();

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

	void CreateBattle2();

};

#endif //!__Battle2_H_