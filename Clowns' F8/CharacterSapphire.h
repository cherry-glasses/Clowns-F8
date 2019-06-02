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
	void SearchAbility_2();
	void SearchAbility_3();

	//void SelectAbility_1();

	void CurrentMovement(MOVEMENT _movement);

};

#endif // !__CharacterSapphire_H__
