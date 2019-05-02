#ifndef _Burgdog_H__
#define _Burgdog_H__

#include "Enemy.h"
#include "Animation.h"

class Burgdog : public Enemy
{
public:
	Burgdog(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Burgdog();

	//Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_1();
	void Ability_1(const std::vector<std::pair<int, int>> *_path);

	void CurrentMovement(MOVEMENT _movement);



};

#endif // !__Boneyman_H__
