#ifndef _CharacterGeorgeB_H__
#define _CharacterGeorgeB_H__

#include "Character.h"

class CharacterGeorgeB : public Character
{
public:

	CharacterGeorgeB(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterGeorgeB();

	//Move and Attack
	void SearchWalk();
	void SearchAttack();
	void SearchAbility_1();
	void SearchAbility_2();
	void SearchAbility_3();

	void CurrentMovement(MOVEMENT _movement);

	//Move and Attack Select
	void InputSelectMove();
	void InputSelectAttack();

};

#endif // !__CharacterGeorgeB_H__