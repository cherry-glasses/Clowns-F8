#ifndef _Polarbear_H__
#define _Polarbear_H__

#include "Enemy.h"
#include "Animation.h"

class Polarbear : public Enemy
{
public:
	Polarbear(ENTITY_TYPE _type, pugi::xml_node _config, int _copy);

	// Destructor
	~Polarbear();

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

