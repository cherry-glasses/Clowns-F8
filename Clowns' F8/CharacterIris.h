#ifndef _CharacterIris_H__
#define _CharacterIris_H__

#include "Character.h"
#include "Animation.h"



class CharacterIris : public Character
{
public:
	CharacterIris(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterIris();

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();


	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	void Wheremove();
	void SearchAttack();
	//bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);

private:
	int Cap = 0;
	int Cap_2 = -1;
	bool flag = false;
	Move_Steps Def = Move_Steps::IDLE;
	Attack_Steps Attk = Attack_Steps::IDLE_A;
	Animation*	current_animation = nullptr;

	std::pair<int, int> possible_att[4];
	std::pair<int, int> possible_mov[8];
	std::pair<int, int> possible_mov_map[8];
	std::pair<int, int> possible_att_map[4];
};

#endif // !__CharacterIris_H__
