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


// cere hay daño critico?
typedef struct {
	int Hp;
	int Mp;
	int Cp;
	int Def;
	int Atk;
	int Crit_hit; // Options to get a critical hit.
} Stats;

//Cere: Deberiamos hacer una estructura de inventario?

class Entity : public ModuleEntityManager
{
public:
	//Falta hacer el creador, pero para eso hemos de tocar el Entity_manager. (cere)
	Entity(/*iPoint pos,*/ Type type, int id);

	// Destructor
	virtual ~Entity() {

	}

	virtual bool PreUpdate() {
		return true;
	}

	virtual void Blit() {

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
	// Vamoh a utilisar un switch (cere).

	virtual void Skill(Entity* objective, int skill_id) {

	}


	//Ponemos los gets y updates aqui=? (cere)
	//A lo get stats y cosas así.
	//hay pasivas(cere)?

//Esto a protected o k?
public:

	int my_id;
	Stats Default_States;
	Stats Current_States;
	Stats Modifiers;
	/*iPoint position;*/
	Type type;
	SDL_Texture* tex = nullptr;
	Animation* animation = nullptr;
	Direction_State e_state;


};



#endif // !_ENTITY_H_

