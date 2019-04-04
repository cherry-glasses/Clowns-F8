#ifndef _Boneyman_H__
#define _Boneyman_H__

#include "Enemy.h"
#include "Animation.h"

class Boneyman : public Enemy
{
public:
	Boneyman(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Boneyman();

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();

	//Move and Attack
	void SearchWalk(const std::vector<std::pair<int, int>> *_path);
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack(const std::vector<std::pair<int, int>> *_path);
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void Hability_1() {}
	void Hability_2() {}
	void Hability_3() {}
	void Die();

	void CurrentMovement(MOVEMENT _movement);

	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


};

#endif // !__Boneyman_H__