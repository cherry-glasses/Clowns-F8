#ifndef _Entity_H__
#define _Entity_H__

#include "Animation.h"

struct SDL_Texture;
struct SDL_Rect;

enum class ENTITY_TYPE
{
	ENTITY_CHARACTER_IRIS,
	ENTITY_ENEMY_HOTDOG,
	ENTITY_ENEMY_BURGDOG,
	NO_TYPE
};

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
	
	Entity(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	virtual ~Entity() {}

	virtual bool Start() { return true;}
	// Called each loop iteration
	virtual bool PreUpdate() {return true;}
	virtual bool Update(float _dt) {return true;}
	virtual bool PostUpdate() {return true;}
	
	//Move and Attack
	virtual void Walk(const std::list<std::pair<int, int>> *_path) {}
	virtual void Attack() {}
	virtual void Hability_1() {}
	virtual void Hability_2() {}
	virtual void Hability_3() {}

	// Load and Save
	virtual bool Load(pugi::xml_node& _node);
	virtual bool Save(pugi::xml_node& _node) const;
	
	// Called before quitting
	virtual bool CleanUp();


	virtual void Draw() {}

	// we can change the Entity* by the id of that entity.
	virtual void Skill(Entity* _objective, int _skill_id) {

	}

	void AddFX(const int _channel, const int _repeat) const;
	bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);

	virtual std::pair<float, float> GetPosition();
	virtual void SetPosition(const float &_x, const float &_y);
	virtual ENTITY_TYPE GetType();

public:

	int my_id;
	Stats Default_States;
	Stats Current_States;
	Stats Modifiers;
	

protected:

	enum MOVEMENT { IDLE, LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN };
	enum STATE { ALIVE, DEATH };

	STATE current_state = ALIVE;
	MOVEMENT last_movement;
	MOVEMENT current_movement = IDLE;

	Animation*	current_animation = nullptr;
	Animation idle;
	Animation walk;
	Animation attack;
	Animation hability_1;
	Animation hability_2;
	Animation dead;

	ENTITY_TYPE type = ENTITY_TYPE::NO_TYPE;
	SDL_Rect	current = { 0,0,0,0 };
	SDL_Texture* entity_texture = nullptr;
	SDL_Texture* debug_texture = nullptr;
	std::string   string_texture;
	std::pair<int, int>  position;
	bool turn = false;
	bool moving = false;
	bool attacking = false;

};

#endif // !_Entity_H_

