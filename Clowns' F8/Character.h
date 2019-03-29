#ifndef __Character_H__
#define __Character_H__

#include "Animation.h"
#include "Entity.h"

class Character : public Entity
{
public:
	Character() : Entity() {}

	// Destructor
	virtual ~Character() {}

	// Called before the first frame
	virtual bool Start() { return false; }

	// Called each loop iteration
	virtual bool PreUpdate() { return false; }
	virtual bool Update(float _dt) { return false; }
	virtual bool PostUpdate() { return false; }

	// Called before quitting
	virtual bool CleanUp() { return false; }

	//Save and Load
	virtual bool Load(pugi::xml_node&) { return false; }
	virtual bool Save(pugi::xml_node&) const { return false; }

public:

	SDL_Texture *char_tex;
};

#endif // !__Character_H__

