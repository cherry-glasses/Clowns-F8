#ifndef __Battle3_H_
#define __Battle3_H_

#include "Battle.h"

struct SDL_Texture;

class Battle3 : public Battle
{
public:
	Battle3(SCENE_TYPE _type, pugi::xml_node& _config);
	~Battle3();

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

	void CreateBattle3();

};

#endif //!__Battle3_H_