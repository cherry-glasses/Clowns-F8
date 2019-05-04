#ifndef __Enemy_H__
#define __Enemy_H__

#include "Animation.h"
#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(ENTITY_TYPE _type, pugi::xml_node _config) : Entity(_type, _config) {}

	// Destructor
	virtual ~Enemy() {}

	// Called before the first frame
	virtual bool Start() { return false; }

	// Called each loop iteration
	virtual bool PreUpdate();
	virtual bool Update(float _dt);
	virtual bool PostUpdate();

	//Move and Attack
	virtual void SearchWalk() {}
	virtual void Walk(const std::vector<std::pair<int, int>> *_path) {}
	virtual void SearchAttack() {}
	virtual void Attack(const std::vector<std::pair<int, int>> *_path) {}
	virtual void SearchAbility_1() {}
	virtual void Ability_1(const std::vector<std::pair<int, int>> *_path) {}
	virtual void SearchAbility_2() {}
	virtual void Ability_2() {}
	virtual void Hability_2() {}
	virtual void Hability_3() {}
	virtual void Die();

	virtual void CurrentMovement(MOVEMENT _movement) {}
	
	// Called before quitting
	virtual bool CleanUp() { return false; }

	//Save and Load
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

protected:
	std::pair<int, int> nearposition;

	//IA
	std::pair<int, int> pos;
	bool inDanger = false;
	bool inRange = false;

	int timer_skill_1 = 0;
	int timer_skill_2 = 0;

};

#endif // !__Enemy_H__
