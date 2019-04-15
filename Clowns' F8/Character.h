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
	virtual bool PreUpdate();
	virtual bool Update(float _dt);
	virtual bool PostUpdate();

	// Character thinks
	virtual void SearchWalk() {}
	virtual void SelectWalk();
	virtual void Walk() {}
	virtual void SearchAttack() {}
	virtual void SelectAttack();
	virtual void Attack() {}
	virtual void Defend();
	void CurrentMovement(MOVEMENT _movement) {}
	void EndTurn();
	void Die();

	//Save and Load
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

	//Moves and Attack Select
	virtual void InputSelectMove() {}
	virtual void InputSelectAttack() {}

protected:
	int tiles_range_attk = 0;
	std::pair<int, int>* range;
	int Cap = 0;
	std::list<std::pair<int, int>> possible_mov_list;
	std::list<std::pair<int, int>> inrange_mov_list;
	std::vector<std::pair<int, int>> possible_map;
};

#endif // !__Character_H__

