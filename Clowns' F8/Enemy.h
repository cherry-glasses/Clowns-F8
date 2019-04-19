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
	virtual bool PreUpdate() { return false; }
	virtual bool Update(float _dt) { return false; }
	virtual bool PostUpdate() { return false; }

	//Move and Attack
	virtual void SearchWalk() {}
	virtual void Walk(const std::vector<std::pair<int, int>> *_path) {}
	virtual void SearchAttack() {}
	virtual void Attack(const std::vector<std::pair<int, int>> *_path) {}
	virtual void Hability_1() {}
	virtual void Hability_2() {}
	virtual void Hability_3() {}
	virtual void Die() {}
	
	// Called before quitting
	virtual bool CleanUp() { return false; }

	//Save and Load
	virtual bool Load(pugi::xml_node&) { return false; }
	virtual bool Save(pugi::xml_node&) const { return false; }

protected:
	std::pair<int, int> nearposition;

};

#endif // !__Enemy_H__
