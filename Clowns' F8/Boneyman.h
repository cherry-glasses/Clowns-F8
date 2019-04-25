#ifndef _Boneyman_H__
#define _Boneyman_H__

#include "Enemy.h"
#include "Animation.h"

class Boneyman : public Enemy
{
public:
	Boneyman(ENTITY_TYPE _type, pugi::xml_node _config, int _copy);

	// Destructor
	~Boneyman();

	//Move and Attack
	void SearchWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void Hability_1() {}
	void Hability_2() {}
	void Hability_3() {}

	void CurrentMovement(MOVEMENT _movement);



};

#endif // !__Boneyman_H__