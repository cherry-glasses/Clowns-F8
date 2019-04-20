#ifndef _BearTrap_H__
#define _BearTrap_H__

#include "Object.h"
#include "Animation.h"

class BearTrap : public Object
{
public:
	BearTrap(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~BearTrap();

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();

	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

};

#endif // !__BearTrap_H__