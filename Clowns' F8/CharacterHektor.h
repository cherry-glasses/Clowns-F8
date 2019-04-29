#ifndef __CharacterHektor_H__
#define __CharacterHektor_H__

#include "Character.h"

class CharacterHektor : public Character
{
public:

	CharacterHektor(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterHektor();

	//Move and Attack
	void SearchWalk();
	void SearchAttack();
	void SearchAbility_1();
	void Hability_2() {}
	void Hability_3() {}

	void CurrentMovement(MOVEMENT _movement);

	//Move and Attack Select
	void InputSelectMove();
	void InputSelectAttack();

};

#endif // !__CharacterHektor_H__
