#ifndef _ENTITY_H__
#define _ENTITY_H__

#include "ModuleEntityManager.h"
#include "Animation.h"

struct SDL_Texture;
struct SDL_Rect;

// to know where is the entity looking.
enum Direction_State {
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	STOP
};


typedef struct {
	int Hp;
	int Mp;
	int Mana;
	int Cp;

	float DefS;
	float AtkS;
	float DefF;
	float AtkF;
	int Crit_hit; // Options to get a critical hit.
} Stats;



class Entity : public ModuleEntityManager
{
public:
	
	Entity(/*iPoint pos,*/ Types type, int id);

	// Destructor
	virtual ~Entity() {

	}

	virtual bool PreUpdate() {
		return true;
	}


	virtual void Draw() {

	}

	virtual bool Update(float dt) {
		return true;
	}

	virtual bool PostUpdate() {
		return true;
	}

	virtual bool Load(pugi::xml_node& node) {
		return true;
	}
	virtual bool Save(pugi::xml_node& node) const {
		return true;

	}

	// we can change the Entity* by the id of that entity.
	virtual void Skill(Entity* objective, int skill_id) {

	}


public:

	int my_id;
	Stats Default_States;
	Stats Current_States;
	Stats Modifiers;
	/*iPoint position;*/
	Types type;
	SDL_Texture* tex = nullptr;
	Animation* animation = nullptr;
	Direction_State e_state;


};



#endif // !_ENTITY_H_

