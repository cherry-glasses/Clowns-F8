#ifndef _CherryBlackGlasses_H__
#define _CherryBlackGlasses_H__

#include "Enemy.h"
#include "Animation.h"

class CherryBlackGlasses : public Enemy
{
public:
	CherryBlackGlasses(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CherryBlackGlasses();

	//Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_1();
	void Ability_1(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_2() {}
	void Ability_2() {}
	std::pair<int, int> Next_cell(std::pair<int, int> pos);


	void CurrentMovement(MOVEMENT _movement);
};

#endif // 

