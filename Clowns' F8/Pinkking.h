#ifndef _Pinkking_H__
#define _Pinkking_H__

#include "Enemy.h"
#include "Animation.h"

class Pinkking : public Enemy
{
public:
	Pinkking(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Pinkking();

	//Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_1();
	void Ability_1(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_2() {}
	void Ability_2() {}

	void CurrentMovement(MOVEMENT _movement);




};

#endif // !__Pinkking_H__