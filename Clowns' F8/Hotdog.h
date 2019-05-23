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

	//void Hability_1() {}
	//void Hability_2() {}
	//void Hability_3() {}

	void CurrentMovement(MOVEMENT _movement);

private:

	bool first = false;
	bool second = false;

};

#endif // !__Hotdog_H__