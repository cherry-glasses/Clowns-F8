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


};

#endif // !__BearTrap_H__