#ifndef _CharacterIris_H__
#define _CharacterIris_H__

#include "Character.h"

class CharacterIris : public Character
{
public:
	CharacterIris(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterIris();

	//Move and Attack
	void SearchWalk();
	void SearchAttack();
	void SearchHability_1();
	void Hability_2() {}
	void Hability_3() {}

	void CurrentMovement(MOVEMENT _movement);

	//Move and Attack Select
	void InputSelectMove();
	void InputSelectAttack();

};

#endif // !__CharacterIris_H__
