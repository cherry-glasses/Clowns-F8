#ifndef _ENTITY_H__
#define _ENTITY_H__

#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Module.h"

struct SDL_Texture;
struct SDL_Rect;

// to know where is the entity looking.

enum class ENTITY_TYPE
{
	ENTITY_CHARACTER,
	ENTITY_ENEMY,
	ENTITY_BOSS,
	NO_TYPE
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



class Entity
{

public:
	
	Entity();

	Entity(ENTITY_TYPE _type);

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

	virtual bool Load(pugi::xml_node& node);
	virtual bool Save(pugi::xml_node& node) const;
	

	virtual bool CleanUp() { return true; };

	// we can change the Entity* by the id of that entity.
	virtual void Skill(Entity* objective, int skill_id) {

	}

	void AddFX(const int _channel, const int _repeat) const;
	bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);

	virtual std::pair<float, float> GetPosition();
	virtual void SetPosition(const float &_x, const float &_y);

public:

	int my_id;
	Stats Default_States;
	Stats Current_States;
	Stats Modifiers;
	ENTITY_TYPE type;
	std::pair<float, float>  position;
	Animation* current_animation = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Rect	current = { 0,0,0,0 };

protected:

	enum MOVEMENT { IDLE, LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN };
	enum STATE { ALIVE, DEATH };

	STATE current_state = ALIVE;
	MOVEMENT last_movement;
	MOVEMENT current_movement = IDLE;

};



#endif // !_ENTITY_H_

