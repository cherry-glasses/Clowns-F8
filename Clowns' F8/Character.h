#ifndef __Character_H__
#define __Character_H__

#include "Animation.h"
#include "Entity.h"

class Character : public Entity
{
public:
	Character(ENTITY_TYPE _type, pugi::xml_node _config) : Entity(_type, _config) {}

	// Destructor
	virtual ~Character() {}

	// Called before the first frame
	virtual bool Start() { return true; }

	// Called each loop iteration
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float _dt) { return true; }
	virtual bool PostUpdate() { return true; }

	// Character thinks
	virtual void SearchAttack() {}

	//Save and Load
	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) const { return true; }

protected:
	int tiles_range_attk = 0;
	std::pair<int, int>* range;
	enum Move_Steps {
		SEARCH,
		SELECT,
		MOVE,
		IDLE
	};

	enum Attack_Steps {
		SELECT_A,
		SEARCH_A,
		ATTACK_A,
		IDLE_A
	};

	//SDL_Texture *char_tex;
};

#endif // !__Character_H__

