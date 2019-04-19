#ifndef _MapLevel1_H__
#define _MapLevel1_H__

#include "Object.h"
#include "Animation.h"

class MapLevel1 : public Object
{
public:
	MapLevel1(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~MapLevel1();

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();

	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

};

#endif // !__MapLevel1_H__