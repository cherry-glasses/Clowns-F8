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
	ENTITY_CHARACTER_GEORGEB,
	ENTITY_ENEMY_BONEYMAN,
	ENTITY_ENEMY_PINKKING,
	ENTITY_ENEMY_HOTDOG,
	ENTITY_ENEMY_BURGDOG,
	NO_TYPE
};

typedef struct {
	int Hp;
	int Mana;
	int PMove;

	int AtkF;
	int AtkS;
	int RangeAtk;
	int RangeAbility_1;
	int RangeAbility_2;
	int RangeAbility_3;
	int DefF;
	int DefS;
	int Crit;
	int Agi;
} Stats;

typedef struct {
	std::string Attack_name;
	std::string Ability_1_name;
	std::string Ability_2_name;
	std::string Ability_3_name;
	std::string Habilidad_1_nombre;
	std::string Habilidad_2_nombre;
	std::string Habilidad_3_nombre;
	
} Attacks_names;

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
	virtual void SearchAttack() {}
	virtual void SearchAbility_1() {}
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
	void LoadAnim(pugi::xml_node _config);
	bool LoadAnimation(pugi::xml_node _node, Animation &_anim);

	virtual std::pair<int, int> GetPosition();
	virtual void SetPosition(const float &_x, const float &_y);
	virtual ENTITY_TYPE GetType();


public:

	Stats default_stats;
	Stats current_stats;
	Attacks_names attacks_names;
	bool defend = false;
	bool stunned = false;
	bool flipX = false;

	enum STATE { ALIVE, DEATH };
	enum TURN { SEARCH_MOVE, SELECT_MOVE, MOVE, SELECT_ACTION, SEARCH_ATTACK, SELECT_ATTACK, ATTACK, 
		SEARCH_ABILITY_1, SELECT_ABILITY_1, ABILITY_1, DEFEND, END_TURN, NONE };

	STATE current_state = ALIVE;
	TURN current_turn = NONE;

	std::pair<int, int>  position_margin;

protected:

	enum MOVEMENT {	IDLE_LEFT_FRONT, IDLE_RIGHT_FRONT, IDLE_LEFT_BACK, IDLE_RIGHT_BACK, IDLE_LEFT, IDLE_RIGHT, IDLE_FRONT, IDLE_BACK,
		WALK_LEFT_FRONT, WALK_RIGHT_FRONT,  WALK_LEFT_BACK, WALK_RIGHT_BACK, WALK_LEFT, WALK_RIGHT, WALK_FRONT, WALK_BACK,
		ATTACK_LEFT_FRONT, ATTACK_RIGHT_FRONT, ATTACK_LEFT_BACK, ATTACK_RIGHT_BACK, ATTACK_LEFT, ATTACK_RIGHT, ATTACK_FRONT, ATTACK_BACK,
		ABILITY_1_LEFT_FRONT, ABILITY_1_RIGHT_FRONT, ABILITY_1_LEFT_BACK, ABILITY_1_RIGHT_BACK, ABILITY_1_LEFT, ABILITY_1_RIGHT, ABILITY_1_FRONT, ABILITY_1_BACK,
		ABILITY_2_LEFT_FRONT, ABILITY_2_RIGHT_FRONT, ABILITY_2_LEFT_BACK, ABILITY_2_RIGHT_BACK, ABILITY_2_LEFT, ABILITY_2_RIGHT, ABILITY_2_FRONT, ABILITY_2_BACK,
		DEFEND_LEFT_FRONT, DEFEND_RIGHT_FRONT, DEFEND_LEFT_BACK, DEFEND_RIGHT_BACK, DEFEND_LEFT, DEFEND_RIGHT, DEFEND_FRONT, DEFEND_BACK,
		DEAD_LEFT_FRONT, DEAD_RIGHT_FRONT, DEAD_LEFT_BACK, DEAD_RIGHT_BACK, DEAD_LEFT, DEAD_RIGHT, DEAD_FRONT, DEAD_BACK,
	};

	MOVEMENT current_movement = IDLE_LEFT_BACK;
	
	Animation*	current_animation = nullptr;
	Animation idle_left_back;
	Animation idle_right_back;
	Animation idle_left_front;
	Animation idle_right_front;
	Animation idle_left;
	Animation idle_right;
	Animation idle_front;
	Animation idle_back;
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
	Animation attack_left;
	Animation attack_right;
	Animation attack_front;
	Animation attack_back;
	Animation ability_1_left_back;
	Animation ability_1_right_back;
	Animation ability_1_left_front;
	Animation ability_1_right_front;
	Animation ability_1_left;
	Animation ability_1_right;
	Animation ability_1_front;
	Animation ability_1_back;
	Animation ability_2_left_back;
	Animation ability_2_right_back;
	Animation ability_2_left_front;
	Animation ability_2_right_front;
	Animation ability_2_left;
	Animation ability_2_right;
	Animation ability_2_front;
	Animation ability_2_back;
	Animation defend_left_back;
	Animation defend_right_back;
	Animation defend_left_front;
	Animation defend_right_front;
	Animation defend_left;
	Animation defend_right;
	Animation defend_front;
	Animation defend_back;
	Animation dead_left_back;
	Animation dead_right_back;
	Animation dead_left_front;
	Animation dead_right_front;
	Animation dead_left;
	Animation dead_right;
	Animation dead_front;
	Animation dead_back;

	ENTITY_TYPE type = ENTITY_TYPE::NO_TYPE;
	SDL_Rect	current = { 0,0,0,0 };
	SDL_Texture* entity_texture = nullptr;
	SDL_Texture* debug_texture = nullptr;
	SDL_Rect debug_green;
	SDL_Rect debug_red;
	SDL_Rect debug_blue;
	std::pair<int, int>  position;
	std::vector<std::pair<int, int>>  objective_position;
	
	
};

#endif // !_Entity_H_

