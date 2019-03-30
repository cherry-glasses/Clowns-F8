#ifndef __Character_H__
#define __Character_H__

#include "Animation.h"
#include "Entity.h"

class Character : public Entity
{
public:
	Character(ENTITY_TYPE _type) : Entity(_type) {}

	// Destructor
	virtual ~Character() {}

	// Called before the first frame
	virtual bool Start() { return true; }

	// Called each loop iteration
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float _dt) { return true; }
	virtual bool PostUpdate() { return true; }

	// Called before quitting
	virtual bool CleanUp() { return true; }

	//Save and Load
	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) const { return true; }

public:

	//SDL_Texture *char_tex;
};

#endif // !__Character_H__

