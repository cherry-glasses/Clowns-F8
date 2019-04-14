#ifndef _Entity_H__
#define _Entity_H__

#include "Animation.h"

struct SDL_Texture;
struct SDL_Rect;

enum class ENTITY_TYPE
{
	ENTITY_CHARACTER_SAPPHIRE,
	ENTITY_CHARACTER_IRIS,
	ENTITY_CHARACTER_STORM,
	ENTITY_ENEMY_BONEYMAN,
	ENTITY_ENEMY_PINKKING,
	ENTITY_ENEMY_HOTDOG,
	ENTITY_ENEMY_BURGDOG,
	NO_TYPE
};

typedef struct {
	int Hp;
	int Mana;
	int Cp;

	int AtkF;
	int AtkS;
	int RangeAtk;
	int DefF;
	int DefS;
	int Crit;
	int Agi;
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
	virtual void SearchWalk() {}
	virtual void Walk(const std::vector<std::pair<int, int>> *_path) {}
	virtual void SearchAttack() {}
	virtual void Attack(const std::vector<std::pair<int, int>> *_path) {}
	virtual void Hability_1() {}
	virtual void Hability_2() {}
	virtual void Hability_3() {}
	virtual void Defend() {}
	virtual void EndTurn() {}
	virtual void Die() {}

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
	bool LoadAnimation(pugi::xml_node _node, Animation &_anim);

	virtual std::pair<int, int> GetPosition();
	virtual void SetPosition(const float &_x, const float &_y);
	virtual ENTITY_TYPE GetType();


public:

	Stats default_stats;
	Stats current_stats;
	bool defend = false;

	enum STATE { ALIVE, DEATH };
	enum TURN { SEARCH_MOVE, SELECT_MOVE, MOVE, SELECT_ACTION, SEARCH_ATTACK, SELECT_ATTACK, ATTACK, 
		SEARCH_HABILITY_1, HABILITY_1, DEFEND, END_TURN, NONE };

	STATE current_state = ALIVE;
	TURN current_turn = NONE;

protected:

	enum MOVEMENT {	IDLE_LEFT_FRONT, IDLE_RIGHT_FRONT, IDLE_LEFT_BACK, IDLE_RIGHT_BACK,
		WALK_LEFT_FRONT, WALK_RIGHT_FRONT,  WALK_LEFT_BACK, WALK_RIGHT_BACK, WALK_LEFT, WALK_RIGHT, WALK_FRONT, WALK_BACK,
		ATTACK_LEFT_FRONT, ATTACK_RIGHT_FRONT, ATTACK_LEFT_BACK, ATTACK_RIGHT_BACK,
		ABILITY_1_LEFT_FRONT, ABILITY_1_RIGHT_FRONT, ABILITY_1_LEFT_BACK, ABILITY_1_RIGHT_BACK,
		ABILITY_2_LEFT_FRONT, ABILITY_2_RIGHT_FRONT, ABILITY_2_LEFT_BACK, ABILITY_2_RIGHT_BACK,
		DEFEND_LEFT_FRONT, DEFEND_RIGHT_FRONT, DEFEND_LEFT_BACK, DEFEND_RIGHT_BACK,
		DEAD_LEFT_FRONT, DEAD_RIGHT_FRONT, DEAD_LEFT_BACK, DEAD_RIGHT_BACK
	};

	MOVEMENT current_movement = IDLE_LEFT_BACK;
	
	Animation*	current_animation = nullptr;
	Animation idle_left_back;
	Animation idle_right_back;
	Animation idle_left_front;
	Animation idle_right_front;
	Animation walk_left_back;
	Animation walk_right_back;
	Animation walk_left_front;
	Animation walk_right_front;
	Animation walk_left;
	Animation walk_right;
	Animation walk_front;
	Animation walk_back;
	Animation attack_left_back;
	Animation attack_right_back;
	Animation attack_left_front;
	Animation attack_right_front;
	Animation hability_1_left_back;
	Animation hability_1_right_back;
	Animation hability_1_left_front;
	Animation hability_1_right_front;
	Animation hability_2_left_back;
	Animation hability_2_right_back;
	Animation hability_2_left_front;
	Animation hability_2_right_front;
	Animation defend_left_back;
	Animation defend_right_back;
	Animation defend_left_front;
	Animation defend_right_front;
	Animation dead_left_back;
	Animation dead_right_back;
	Animation dead_left_front;
	Animation dead_right_front;

	ENTITY_TYPE type = ENTITY_TYPE::NO_TYPE;
	SDL_Rect	current = { 0,0,0,0 };
	SDL_Texture* entity_texture = nullptr;
	SDL_Texture* debug_texture = nullptr;
	SDL_Rect debug_green;
	SDL_Rect debug_red;
	SDL_Rect debug_blue;
	std::pair<int, int>  position;
	std::pair<int, int>  position_margin;
	std::vector<std::pair<int, int>>  objective_position;
	
	
};

#endif // !_Entity_H_

