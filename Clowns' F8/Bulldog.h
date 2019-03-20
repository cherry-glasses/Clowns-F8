#ifndef _Bulldog_H__
#define _Bulldog_H__

#include "Enemy.h"
#include "Animation.h"

class Bulldog : public Enemy
{
public:
	Bulldog();

	// Destructor
	~Bulldog();

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

#endif // !__Bulldog_H__