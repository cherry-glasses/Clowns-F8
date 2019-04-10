#ifndef _CharacterSapphire_H__
#define _CharacterSapphire_H__

#include "Character.h"
#include "Animation.h"



class CharacterSapphire : public Character
{
public:
	CharacterSapphire(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterSapphire();

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();

	//Move and Attack
	void SearchWalk();
	void SelectWalk();
	void Walk(const std::vector<std::pair<int, int>> *_path);
	void SearchAttack();
	void Attack(const std::vector<std::pair<int, int>> *_path);
	void Hability_1() {}
	void Hability_2() {}
	void Hability_3() {}
	void Die();

	void CurrentMovement(MOVEMENT _movement);

	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	//bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);

private:
	int Cap = 0;

	std::pair<int, int> possible_att[4];
	std::pair<int, int> possible_mov[8];
	std::pair<int, int> possible_mov_map[8];
	std::pair<int, int> possible_att_map[4];
};

#endif // !__CharacterSapphire_H__
