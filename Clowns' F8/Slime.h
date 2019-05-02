#ifndef _Slime_H__
#define _Slime_H__

#include "Enemy.h"
#include "Animation.h"

class Slime : public Enemy
{
public:
	Slime(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Slime();

	//Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void SearchAbility_1();
	void Ability_1(const std::vector<std::pair<int, int>> *_path);

	void CurrentMovement(MOVEMENT _movement);



};

#endif // pawn

