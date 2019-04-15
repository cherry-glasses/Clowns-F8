#ifndef _CharacterStorm_H__
#define _CharacterStorm_H__

#include "Character.h"

class CharacterStorm : public Character
{
public:

	CharacterStorm(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterStorm();

	//Move and Attack
	void SearchWalk();
	void SearchAttack();
	void Hability_1() {}
	void Hability_2() {}
	void Hability_3() {}

	void CurrentMovement(MOVEMENT _movement);

	//Move and Attack Select
	void InputSelectMove();
	void InputSelectAttack();

};

#endif // !__CharacterStorm_H__
