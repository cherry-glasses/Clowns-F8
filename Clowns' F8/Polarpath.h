#ifndef _Polarpath_H__
#define _Polarpath_H__

#include "Enemy.h"
#include "Animation.h"

class Polarpath : public Enemy
{
public:
	Polarpath(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Polarpath();

	//Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_1();
	void Ability_1(const std::vector<std::pair<int, int>> *_path);

	void CurrentMovement(MOVEMENT _movement);



};

#endif //!__Polarpath_H__

