#ifndef _Entity_H__
#define _Entity_H__

#include "Animation.h"

struct SDL_Texture;
struct SDL_Rect;

typedef struct {
	int Hp;
	int Mp;
	int Mana;
	int Cp;

	int DefS;
	int AtkS;
	int DefF;
	int AtkF;
	int Crit_hit; // Options to get a critical hit.
} Stats;

class Entity
{

public:
	
	Entity();

	// Destructor
	virtual ~Entity() {}

	// Called each loop iteration
	virtual bool PreUpdate() {return true;}
	virtual bool Update(float _dt) {return true;}
	virtual bool PostUpdate() {return true;}


	virtual bool Load(pugi::xml_node& _node);
	virtual bool Save(pugi::xml_node& _node) const;
	
	// Called before quitting
	virtual bool CleanUp() { return true;}


	virtual void Draw() {}

	// we can change the Entity* by the id of that entity.
	virtual void Skill(Entity* _objective, int _skill_id) {

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

#endif // !_Entity_H_

