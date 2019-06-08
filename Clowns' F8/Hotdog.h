#ifndef _Hotdog_H__
#define _Hotdog_H__

#include "Enemy.h"
#include "Animation.h"

class Hotdog : public Enemy
{
public:
	Hotdog(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Hotdog();

	//Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_1();
	void Ability_1(const std::vector<std::pair<int, int>> *_path);
	//void Hability_1() {}
	//void Hability_2() {}
	//void Hability_3() {}

	void CurrentMovement(MOVEMENT _movement);

private:

	bool first = false;
	bool second = false;

	std::pair<int, int> they_see_me_rolling;
	bool is_fucking_walkeable;
	int ability_1_range = 3;

};

#endif // !__Hotdog_H__