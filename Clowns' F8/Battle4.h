#ifndef __Battle4_H_
#define __Battle4_H_

#include "Battle.h"

struct SDL_Texture;

class Battle4 : public Battle
{
public:
	Battle4(SCENE_TYPE _type, pugi::xml_node& _config);
	~Battle4();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called before all Updates
	bool PostUpdate(float dt);

	// Load and Save
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

private:

	void CreateBattle4();

};

#endif //!__Battle4_H_