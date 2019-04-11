#ifndef _CharacterStorm_H__
#define _CharacterStorm_H__

#include "Character.h"
#include "Animation.h"



class CharacterStorm : public Character
{
public:

	CharacterStorm(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterStorm();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float _dt);
	bool PostUpdate();


	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	void SearchWalk();
	void SearchAttack();
	void Die();


private:
	int Cap = 0;
	int Cap_2 = 1;
	std::pair<int, int>* movment_1;
	std::pair<int, int>* movment_2;
	std::pair<int, int>* movment_3;
	std::pair<int, int>* movment_4;
	Move_Steps Def = Move_Steps::IDLE;
	Attack_Steps Attk = Attack_Steps::IDLE_A;
	Animation*	current_animation = nullptr;

};
#endif
