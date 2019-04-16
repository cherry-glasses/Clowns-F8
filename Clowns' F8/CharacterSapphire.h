#ifndef _CharacterSapphire_H__
#define _CharacterSapphire_H__

#include "Character.h"

class CharacterSapphire : public Character
{
public:
	CharacterSapphire(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterSapphire();

	//Move and Attack
	void SearchWalk();
	void SearchAttack();
	void SearchAbility_1();

	void CurrentMovement(MOVEMENT _movement);

	//Move and Attack Select
	void InputSelectMove();
	void InputSelectAttack();

};

#endif // !__CharacterSapphire_H__
