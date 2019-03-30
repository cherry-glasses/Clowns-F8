#ifndef _Hotdog_H__
#define _Hotdog_H__

#include "Enemy.h"
#include "Animation.h"

class Hotdog : public Enemy
{
public:
	Hotdog(ENTITY_TYPE _type);

	// Destructor
	~Hotdog();

	// Called before render is available
	bool	Awake(pugi::xml_node & _config);

	// Called before the first frame
	bool	Start(uint _i);

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();

	//Move and Attack
	void Walk(const std::list<std::pair<int, int>> *_path);
	void Attack() {}
	void Hability_1() {}
	void Hability_2() {}
	void Hability_3() {}

	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);
private:

	bool CleanUp();

};

#endif // !__Hotdog_H__