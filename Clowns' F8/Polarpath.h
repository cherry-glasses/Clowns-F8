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

	////Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_1();
	void Ability_1(const std::vector<std::pair<int, int>> *_path);
	//void SearchAbility_2() {}
	//void Ability_2() {}
	//std::pair<int, int> Next_cell(std::pair<int, int> pos);

	void CurrentMovement(MOVEMENT _movement);

private:
	std::pair<int, int> abil_1_arr[8];
};

#endif

